/**
 * temp_test_harness.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "temp_test_harness.h"
#include "extraction/extract_conversations_generic.h"
#include "sources/atomic/AtomicConversationSource.h"
#include "sources/atomic/FileConversationSource.h"
#include "sources/atomic/LibArchiveEntryConversationSource.h"
#include "intermediate_format/subjects/FullySpecifiedSubject.h"
#include "intermediate_format/subjects/AccountSubject.h"
#include "intermediate_format/subjects/ScreenNameSubject.h"
#include "intermediate_format/subjects/ResolvedSubject.h"
#include "intermediate_format/RawConversationCollection.h"
#include "fixers/resolve_subjects/resolve_subjects.h"
#include "fixers/resolve_subjects/ResolveSubjectsConfig.h"
#include "utils/json/load_json.h"
#include "utils/json/json_utils.h"
#include "utils/qt/shortcuts.h"
#include "utils/qt/debug_extras.h"
#include "utils/language/invariant.h"
#include "utils/language/shortcuts.h"

#include <archive.h>
#include <archive_entry.h>

#include <QDebug>
#include <QDirIterator>
#include <QString>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QThreadPool>
#include <QRunnable>

#include <map>
#include <set>
#include <vector>

namespace uniarchive2 {

using namespace std;
using namespace uniarchive2::extraction;
using namespace uniarchive2::sources;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::fixers::resolve_subjects;
using namespace uniarchive2::utils::json;


typedef map<QString, QString> variables_t;


QString remove_trailing_slash(IMM(QString) path) {
    return path.endsWith("/") ? path.left(path.length() - 1) : path;
}

QString parse_maybe_new_directory(QJsonValue json_value) {
    invariant(!json_value.isUndefined(), "Missing value for directory");
    invariant(json_value.isString(), "Directory should be string");

    QString dir = remove_trailing_slash(json_value.toString());

    return dir;
}

QString parse_existing_directory(QJsonValue json_value) {
    QString dir = parse_maybe_new_directory(json_value);

    invariant(QDir(dir).exists(), "Directory '%s' does not exist", QP(dir));

    return dir;
}

QString parse_maybe_new_filename(QJsonValue json_value) {
    invariant(!json_value.isUndefined(), "Missing value for filename");
    invariant(json_value.isString(), "Filename should be string");

    return json_value.toString();
}

QString parse_existing_filename(QJsonValue json_value) {
    QString filename = parse_maybe_new_filename(json_value);

    invariant(QFile(filename).exists(), "File '%s' does not exist", QP(filename));

    return filename;
}

set<ArchiveFormat> parse_formats_set(QJsonValue json_value) {
    set<ArchiveFormat> formats;

    if (json_value.isUndefined()) {
        return formats;
    }
    if (json_value.isArray()) {
        for (IMM(auto) item : json_value.toArray()) {
            invariant(item.isString(), "Format array value should be JSON string");
            formats.insert(archive_format_from_symbol(item.toString()));
        }
        return formats;
    }
    if (json_value.isObject()) {
        QJsonObject as_object = json_value.toObject();
        for (IMM(auto) key : as_object.keys()) {
            invariant(as_object[key].isBool(), "Format dictionary value should have bools as values");
            if (as_object[key].toBool()) {
                formats.insert(archive_format_from_symbol(key));
            }
        }
        return formats;
    }

    invariant_violation("Invalid format for formats set");

    return formats;
}

class ExtractConversationsTask : public QRunnable {
public:
    ArchiveFormat format;
    unique_ptr<AtomicConversationSource> source;

    RawConversationCollection result;

    ExtractConversationsTask(ArchiveFormat format, TAKE(AtomicConversationSource) source)
      : format(format), source(move(source)) {
        setAutoDelete(false);
    }

    void run() {
        result.take(extract_conversations_generic(format, *source));
    }
};

void make_tasks_using_folder(
    vector<unique_ptr<ExtractConversationsTask>>& mut_tasks,
    ArchiveFormat format,
    IMM(QString) base_input_path,
    IMM(QString) subpath,
    IMM(QString) glob
) {
    QDirIterator files(
        base_input_path + "/" + subpath,
        QStringList() << glob,
        QDir::Files,
        QDirIterator::Subdirectories
    );

    while (files.hasNext()) {
        QString filename = files.next();
        unique_ptr<AtomicConversationSource> source = make_unique<FileConversationSource>(filename);
        mut_tasks.push_back(make_unique<ExtractConversationsTask>(format, move(source)));
    }
}

void make_tasks_using_archive(
    vector<unique_ptr<ExtractConversationsTask>>& mut_tasks,
    ArchiveFormat format,
    IMM(QString) archive_filename,
    IMM(QString) glob
) {
    QRegExp pattern(glob, Qt::CaseSensitive, QRegExp::WildcardUnix);

    struct archive *arch;
    struct archive_entry *entry;

    arch = archive_read_new();
    archive_read_support_filter_all(arch);
    archive_read_support_format_all(arch);

    int status = archive_read_open_filename(arch, QP(archive_filename), 10240);
    invariant(status == ARCHIVE_OK, "Error opening archive: %s", QP(archive_filename));

    while (archive_read_next_header(arch, &entry) == ARCHIVE_OK) {
        if (archive_entry_filetype(entry) != AE_IFREG) {
            continue;
        }

        QString filename = QString::fromUtf8(archive_entry_pathname_utf8(entry));
        if (!pattern.exactMatch(filename)) {
            continue;
        }

        unique_ptr<AtomicConversationSource> source =
            make_unique<LibArchiveEntryConversationSource>(archive_filename, arch, entry);
        mut_tasks.push_back(make_unique<ExtractConversationsTask>(format, move(source)));
    }

    archive_read_free(arch);
}

RawConversationCollection extract_conversations(
    IMM(QString) base_input_path,
    set<ArchiveFormat> include_formats = set<ArchiveFormat>(),
    set<ArchiveFormat> exclude_formats = set<ArchiveFormat>()
) {
    RawConversationCollection convos;

    QThreadPool* tp = QThreadPool::globalInstance();

    unsigned int n_threads = QThread::idealThreadCount() * 2; // oversubscribe 2x

    qDebug() << "Using" << n_threads << "threads";

    tp->setMaxThreadCount(n_threads);

    vector<tuple<ArchiveFormat, QString, QString>> inputs {
        { ArchiveFormat::SKYPE,               "skype",    "main.db" },
        { ArchiveFormat::WHATSAPP_EMAIL,      "whatsapp", "*.txt" },
        { ArchiveFormat::PIDGIN_TXT,          "pidgin",   "*.txt" },
        { ArchiveFormat::PIDGIN_HTML,         "pidgin",   "*.html" },
        { ArchiveFormat::ADIUM,               "adium",    "*).xml" },
        { ArchiveFormat::DIGSBY,              "digsby",   "*.html" },
        { ArchiveFormat::FACEBOOK_DYI,        "facebook", "messages.htm" },
        { ArchiveFormat::YAHOO_MESSENGER_DAT, "yahoo",    "*.dat" },
        { ArchiveFormat::MSN_MESSENGER_XML,   "msn",      "*xml" },
    };

    for (IMM(auto) format_path_glob : inputs) {
        ArchiveFormat format = get<ArchiveFormat>(format_path_glob);

        if (!include_formats.empty() && (include_formats.find(format) == include_formats.end())) {
            continue;
        }
        if (exclude_formats.find(format) != exclude_formats.end()) {
            continue;
        }

        qDebug() << "    - Extracting:" << format;

        vector<unique_ptr<ExtractConversationsTask>> tasks;

        QString subpath = get<1>(format_path_glob);
        QString glob = get<2>(format_path_glob);

        if (QFile(base_input_path + "/" + subpath + ".tar").exists()) {
            make_tasks_using_archive(tasks, format, base_input_path + "/" + subpath + ".tar", glob);
        } else {
            make_tasks_using_folder(tasks, format, base_input_path, subpath, glob);
        }

        for (IMM(auto) task : tasks) {
            tp->start(task.get());
        }

        tp->waitForDone();

        for (IMM(auto) task : tasks) {
            convos.take(move(task->result));
        }
    }

    return convos;
}

QString base_filename_for_conversation(IMM(RawConversation) convo, int discriminant = 0) {
    QString filename;
    QDebug ss(&filename);

    ss.nospace();
    if (convo.declaredStartDate) {
        ss << " " << *convo.declaredStartDate;
    } else if (!convo.events.empty()) {
        ss << " " << convo.events.front()->timestamp;
    } else {
        ss << " (Unknown date)";
    }

    if (discriminant) {
        ss << " (" << discriminant << ")";
    }

    ss << ".txt";

    filename.replace(':', '.');

    return filename.trimmed();
}

void dump_conversations(IMM(RawConversationCollection) conversations, IMM(QString) output_dir) {
    map<QString, uint> filenames_used;

    for (IMM(auto) convo : conversations) {
        QStringList convo_path;
        convo_path << name_for_im_protocol(convo.protocol);

        if (
            (convo.isConference && *convo.isConference) ||
            (!convo.isConference && convo.declaredPeers.size() > 1)
            ) {
            convo_path << "Conferences";
        }
        if (convo.declaredPeers.empty()) {
            convo_path << "(Unknown)";
        } else {
            ApparentSubject* subject = convo.declaredPeers.front().get();
            if (subject->is<FullySpecifiedSubject>()) {
                convo_path << ("_" + subject->as<FullySpecifiedSubject>()->accountName.accountName);
            } else if (subject->is<AccountSubject>()) {
                convo_path << ("_" + subject->as<AccountSubject>()->account.accountName);
            } else if (subject->is<ScreenNameSubject>()) {
                convo_path << ("_" + subject->as<ScreenNameSubject>()->screenName);
            } else if (subject->is<ResolvedSubject>()) {
                convo_path << subject->as<ResolvedSubject>()->canonicalSubjectID;
            } else {
                convo_path << "(Unknown)";
            }
        }

        QDir(output_dir).mkpath(convo_path.join(QDir::separator()));

        QString destination_dir =
           output_dir + QDir::separator() + convo_path.join(QDir::separator()) + QDir::separator();

        QString full_filename = destination_dir + base_filename_for_conversation(convo);

        filenames_used[full_filename]++;
        if (filenames_used[full_filename] > 1) {
            full_filename = destination_dir + base_filename_for_conversation(convo, filenames_used[full_filename]);
        }

        QFile f(full_filename);
        f.open(QFile::WriteOnly);
        invariant(f.isWritable(), "Could not open output file '%s'", QP(full_filename));

        QDebug writer(&f);
        convo.writeToDebugStream(writer, true);
    }
}

void sanity_check_dumped_conversations_dir(IMM(QString) output_dir) {
    QStringList allowed_dirs { "Digsby", "Facebook", "Jabber", "MSN", "Skype", "WhatsApp", "Yahoo" };

    for (IMM(auto) entry : QDir(output_dir).entryList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
        invariant(
            allowed_dirs.contains(entry),
            "Error: '%s' does not seem to be a folder where conversations were dumped (it contains unrecognized "
            "entry '%s'). Aborting for safety.",
            QP(output_dir),
            QP(entry)
        );
    }
}

void clear_dumped_conversations(IMM(QString) output_dir) {
    sanity_check_dumped_conversations_dir(output_dir);

    for (IMM(auto) entry : QDir(output_dir).entryList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
        QString subpath = output_dir + "/" + entry;
        invariant(QDir(subpath).removeRecursively(), "Failed to remove '%s'", QP(subpath));
    }
}

void load_or_merge_convos(
    RawConversationCollection& convos,
    RawConversationCollection&& new_convos,
    IMM(QJsonObject) command_obj
) {
    auto merge_value = command_obj["merge"];

    if (!merge_value.isUndefined()) {
        invariant(merge_value.isBool(), "Key 'merge' should be a boolean");
        if (merge_value.toBool()) {
            convos.take(move(new_convos));
            return;
        }
    }

    convos = move(new_convos);
}

void run_extract_conversations_command(IMM(QJsonObject) command_obj, RawConversationCollection& convos) {
    QString base_path = parse_existing_directory(command_obj["base_path"]);

    qDebug() << "Extracting conversations at:" << QP(base_path);

    load_or_merge_convos(
        convos,
        extract_conversations(
            base_path,
            parse_formats_set(command_obj["include_formats"]),
            parse_formats_set(command_obj["exclude_formats"])
        ),
        command_obj
    );
}

void run_dump_conversations_command(IMM(QJsonObject) command_obj, IMM(RawConversationCollection) convos) {
    QString output_dir = parse_maybe_new_directory(command_obj["output_dir"]);

    qDebug() << "Dumping conversations to:" << QP(output_dir);

    if (command_obj["clear_before"].isBool() && command_obj["clear_before"].toBool()) {
        clear_dumped_conversations(output_dir);
    }

    dump_conversations(convos, output_dir);
}

void run_resolve_subjects_command(IMM(QJsonObject) command_obj, RawConversationCollection& mut_convos) {
    QString config_filename = parse_existing_filename(command_obj["config_filename"]);

    qDebug() << "Applying fix: Resolve subjects";

    resolve_subjects(mut_convos, ResolveSubjectsConfig::loadFromFile(config_filename));
}

void run_clear_dumped_conversations_command(IMM(QJsonObject) command_obj) {
    QString output_dir = parse_maybe_new_directory(command_obj["output_dir"]);

    qDebug() << "Clearing dumped conversations from:" << QP(output_dir);

    clear_dumped_conversations(output_dir);
}

void run_load_conversations_binary_command(IMM(QJsonObject) command_obj, RawConversationCollection& convos) {
    QString filename = parse_existing_filename(command_obj["filename"]);

    qDebug() << "Loading conversations from binary file:" << QP(filename);

    load_or_merge_convos(
        convos,
        RawConversationCollection::loadFromBinaryFile(filename),
        command_obj
    );
}

void run_save_conversations_binary_command(IMM(QJsonObject) command_obj, IMM(RawConversationCollection) convos) {
    QString filename = parse_maybe_new_filename(command_obj["filename"]);

    qDebug() << "Saving conversations to binary file:" << QP(filename);

    convos.writeToBinaryFile(filename);
}

void run_commands(QJsonValue commands_json) {
    RawConversationCollection convos;

    for (IMM(auto) command_json : expect_json_array(commands_json, "'commands' key in config")) {
        QJsonObject command_obj = expect_json_object(command_json, "Command JSON");

        if (command_obj.contains("skip")) {
            invariant(command_obj["skip"].isBool(), "'skip' key should be boolean");
            if (command_obj["skip"].toBool()) {
                continue;
            }
        }

        QString command = expect_json_string(command_obj["command"], "'command' key");

        if (command == "extract_conversations") {
            run_extract_conversations_command(command_obj, convos);
        } else if (command == "dump_conversations") {
            run_dump_conversations_command(command_obj, convos);
        } else if (command == "clear_dumped_conversations") {
            run_clear_dumped_conversations_command(command_obj);
        } else if (command == "load_conversations_binary") {
            run_load_conversations_binary_command(command_obj, convos);
        } else if (command == "save_conversations_binary") {
            run_save_conversations_binary_command(command_obj, convos);
        } else if (command == "resolve_subjects") {
            run_resolve_subjects_command(command_obj, convos);
        } else {
            invariant_violation("Unsupported command: '%s'", QP(command));
        }
    }
}

QString apply_variables(IMM(QString) format, IMM(variables_t) variables) {
    QREGEX(PATTERN, "<([^>]+)>");

    QString value;
    int last_written_pos = 0;

    auto matches = PATTERN.globalMatch(format);
    while (matches.hasNext()) {
        auto match = matches.next();
        IMM(auto) var_name = match.captured(1);

        value.append(format.midRef(last_written_pos, match.capturedStart() - last_written_pos));
        last_written_pos = match.capturedEnd();

        invariant(variables.count(var_name), "Variable '%s' not found", QP(var_name));

        value.append(variables.at(var_name));
    }

    value.append(format.midRef(last_written_pos));

    return value;
}

QJsonValue apply_variables(IMM(QJsonValue) value, IMM(variables_t) variables) {
    if (value.isArray()) {
        IMM(QJsonArray) asArray = value.toArray();
        QJsonArray result;

        for (IMM(auto) item : asArray) {
            result.push_back(apply_variables(item, variables));
        }

        return result;
    }
    if (value.isObject()) {
        IMM(QJsonObject) asObject = value.toObject();
        QJsonObject result;

        for (IMM(auto) key : asObject.keys()) {
            result[key] = apply_variables(asObject[key], variables);
        }

        return result;
    }
    if (value.isString()) {
        return apply_variables(value.toString(), variables);
    }

    return value;
}

variables_t read_variables(QJsonValue vars_json) {
    variables_t variables;

    if (vars_json.isUndefined()) {
        return variables;
    }

    map<QString, QString> definitions;
    auto as_obj = expect_json_object(vars_json, "'variables' key in config");

    for (IMM(QString) name : as_obj.keys()) {
        definitions[name] = expect_json_string(as_obj[name], "Value for variable '" + name + "'");
        variables[name] = "";
    }

    for (int max_iterations = 20; max_iterations > 0; max_iterations--) {
        bool changed = false;

        for (IMM(auto) defs_kv : definitions) {
            IMM(QString) name = defs_kv.first;
            IMM(QString) old_value = variables[name];
            IMM(QString) new_value = apply_variables(defs_kv.second, variables);

            if (new_value != old_value) {
                variables[name] = new_value;
                changed = true;
            }
        }

        if (!changed) {
            break;
        }
    }

    return variables;
}

void run_test_harness(IMM(QString) config_file) {
    QJsonDocument doc = load_json_file(config_file);
    QJsonObject config_object = expect_json_object(doc, "Root value in config JSON");

    variables_t variables = read_variables(config_object["variables"]);

    run_commands(apply_variables(config_object["commands"], variables));
}

}
