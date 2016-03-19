#include "debug/conversations.h"

#include <iostream>

#define CONTEXT_LINES 3


static void highlight_messages_in_conv(const RawConversation *conversation,
                                       std::vector<char> markings)
{
    int show_lines_counter = 0;
    bool shown_elipsis = false;

    conversation->dump(false);

    for (int i = -CONTEXT_LINES; i < (int)markings.size(); i++) {
        if ((i + CONTEXT_LINES < (int)markings.size()) &&
             markings[i + CONTEXT_LINES]) {
            show_lines_counter = 2 * CONTEXT_LINES + 1;
        }

        if (show_lines_counter) {
            if (i >= 0) {
                std::cout << "  "
                          << (markings[i] ? markings[i] : ' ')
                          << " "
                          << qPrintable(conversation->messages[i]->description())
                          << std::endl;
            }
            show_lines_counter--;
            shown_elipsis = false;
        } else if (!shown_elipsis) {
            std::cout << std::endl << "    ..." << std::endl << std::endl;
            shown_elipsis = true;
        }
    }
}

bool find_temporal_discontinuities(const RawConversation *conversation)
{
    std::vector<char> markings;
    bool problems_found = false;

    TimeStamp prev_timestamp;
    for (const RawMessageUqPtr& message : conversation->messages) {
        if (!prev_timestamp.isNull() && (message->date < prev_timestamp)) {
            markings.push_back('*');
        } else {
            markings.push_back(0);
        }

        prev_timestamp = message->date;
    }

    if (problems_found) {
        highlight_messages_in_conv(conversation, markings);
    }

    return problems_found;
}
