/**
 * fixers/resolve_subjects/ResolveSubjectsConfig.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_FIXERS_RESOLVESUBJECTS_RESOLVESUBJECTSCONFIG_H
#define UNIARCHIVE2_FIXERS_RESOLVESUBJECTS_RESOLVESUBJECTSCONFIG_H

#include "utils/language/shortcuts.h"

#include <QString>

namespace uniarchive2 { namespace fixers { namespace resolve_subjects {

struct ResolveSubjectsConfig {
    static ResolveSubjectsConfig loadFromFile(IMM(QString) filename);
};

}}}

#endif //UNIARCHIVE2_FIXERS_RESOLVESUBJECTS_RESOLVESUBJECTSCONFIG_H
