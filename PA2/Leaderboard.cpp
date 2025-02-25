#include "Leaderboard.h"

// Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
// variable will point to the highest all-times score, and all other scores will be reachable from it
// via the "next_leaderboard_entry" member variable pointer.
void Leaderboard::read_from_file(const string &filename) {
    // TODO: Your code here
}


// Write the latest leaderboard status to the given file in the format specified in the PA instructions
void Leaderboard::write_to_file(const string &filename) {
    // TODO: Your code here
}

// Print the current leaderboard status to the standard output in the format specified in the PA instructions
void Leaderboard::print_leaderboard() {
    // TODO: Your code here
}

//  Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
//  is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
//  top 10 all-time high-scores should be kept in descending order by the score).
void Leaderboard::insert(LeaderboardEntry *new_entry) {
    // TODO: Your code here
    if (head_leaderboard_entry == nullptr) {
        head_leaderboard_entry = new_entry;
    } else {
        if (new_entry->score > head_leaderboard_entry->score) {
            LeaderboardEntry* temp = head_leaderboard_entry;
            head_leaderboard_entry = new_entry;
            new_entry->next = temp;
        } else {
            LeaderboardEntry* current = head_leaderboard_entry;
            while (true) {
                if (current->next == nullptr || new_entry->score > current->next->score) {
                    break;
                }
                current = current->next;
            }
            new_entry->next = current->next;
            current->next = new_entry;
        }
    }
    
}

// Free dynamically allocated memory used for storing leaderboard entries
Leaderboard::~Leaderboard() {
    // TODO: Your code here
}
