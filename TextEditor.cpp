#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class TextEditor {
private:
    string text;
    int cursor_pos;
    vector<string> clipboard;
    bool has_selection;
    int select_start;
    int select_end;


    void clearSelection() {
        has_selection = false;
        select_start = 0;
        select_end = 0;
    }


    void replaceSelectionIfActive() {
        if (has_selection) {
            text.erase(select_start, select_end - select_start + 1);
            cursor_pos = select_start;
            clearSelection();
        }
    }


    int clampCursor(int pos) const {
        if (pos < 0) {
            return 0;
        }
        else if (pos > text.length()) {
            return text.length();
        }
        
        return pos;
    }


public:
    // Constructor
    TextEditor() {
        text = "";
        cursor_pos = 0;
        has_selection = false;
        select_start = 0;
        select_end = 0;
    }


    void type(const string& new_text) {
        replaceSelectionIfActive();
        text.insert(cursor_pos, new_text);
        cursor_pos += new_text.length();
    }


    void select(int start_index, int end_index) {
        has_selection = true;
        select_start = start_index;
        select_end = end_index;
        cursor_pos = clampCursor(end_index + 1); // l'enunciat assegura que end_index serà vàlid, però per bona pràctica clampo igualment
    }


    void moveCursor(int offset) {
        clearSelection();
        cursor_pos = clampCursor(cursor_pos + offset);
    }


    void copy() {
        if (has_selection) {
            clipboard.push_back(text.substr(select_start, select_end - select_start + 1));
        }
    }


    void paste(int steps_back = 1) {
        if (steps_back > (int)clipboard.size() || steps_back <= 0) {
            return; 
        }
        int index_to_paste = clipboard.size() - steps_back;
        type(clipboard[index_to_paste]);
    }

    
    string executeCommands(const vector<string>& operations) {
        for (const string& op : operations) {
            size_t space_pos = op.find(' ');
            string cmd = op.substr(0, space_pos);

            if (cmd == "EXIT") {
                break;
            } 
            else if (cmd == "TYPE") {
                string text_to_type = op.substr(space_pos + 1);
                type(text_to_type);
            } 
            else if (cmd == "SELECT") {
                stringstream ss(op.substr(space_pos + 1));
                int start, end;
                ss >> start >> end;
                select(start, end);
            } 
            else if (cmd == "MOVE_CURSOR") {
                int offset = stoi(op.substr(space_pos + 1));
                moveCursor(offset);
            } 
            else if (cmd == "COPY") {
                copy();
            } 
            else if (cmd == "PASTE") {
                if (space_pos != string::npos) {
                    int steps = stoi(op.substr(space_pos + 1));
                    paste(steps);
                } else {
                    paste(); 
                }
            }
        }
        return text;
    }
};


int main() {
    TextEditor editor;
    vector<string> operations;
    string line;

    while (getline(cin, line)) {
        // possible problema amb salt de línia a Windows
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        
        if (line.empty()) continue;

        operations.push_back(line);

        if (line == "EXIT") {
            break;
        }
    }

    string final_text = editor.executeCommands(operations);
    
    cout << "\"" << final_text << "\"" << endl;

    return 0;
}