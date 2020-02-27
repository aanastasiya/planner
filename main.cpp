#include <iostream>
#include <iomanip>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <regex>
using namespace std;

class Date {
public:
    Date() : year(0), month(0), day(1) {}
    Date(const int& y, const int& m, const int& d) {
        if ((m < 1) || (m > 12)) {
            throw invalid_argument("Month value is invalid: " + to_string(m));
        }
        else if (((d < 1) || (d > 31)) && ((m < 1) || (m > 12))) {
            throw invalid_argument("Month value is invalid: " + to_string(m));
        }
        else if ((d < 1) || (d > 31)) {
            throw invalid_argument("Day value is invalid: " + to_string(d));
        }
        else {
            year = y;
            month = m;
            day = d;
        }
    }

    int GetYear() const {
        return year;
    }
    int GetMonth() const {
        return month;
    }
    int GetDay() const {
        return day;
    }
private:
    int year, month, day;
};

vector<string> ParseCommand (string& commandString) {
    stringstream stream(commandString);
    string command, date, event;
    stream >> command;
    stream.ignore(1);
    stream >> date;
    stream.ignore(1);
    stream >> event;
    return {command, date, event};
}

Date ConvertToDate (string& dateString) {
    /*regex reg("([-+]?[0-9]+)-([-+]?[0-9]+)-([-+]?[0-9]+)$");
    smatch sm;
    if (!regex_match(dateString, sm, reg)) {
        throw invalid_argument("Wrong date format: " + dateString);
    }
    else {
        return Date(stoi(sm[1]), stoi(sm[2]), stoi(sm[3]));
    }*/

    stringstream stream(dateString);
        int y, m, d;
        char firstDivider, secondDivider;
        stream >> y;
        stream >> firstDivider;
        stream >> m;
        stream >> secondDivider;
        stream >> d;
        if (!stream.eof() || !stream) {
            throw invalid_argument("Wrong date format: " + dateString);
        }
        if (firstDivider == '-' && secondDivider == '-') {
            return Date(y, m, d);
        }
        else {
            throw invalid_argument("Wrong date format: " + dateString);
        }
}

bool operator<(const Date& lhs, const Date& rhs) {
    if (lhs.GetYear() < rhs.GetYear()) {
        return true;
    }
    else if (lhs.GetYear() == rhs.GetYear()) {
        if (lhs.GetMonth() < rhs.GetMonth()) {
            return true;
        }
        else if (lhs.GetMonth() == rhs.GetMonth()) {
            if (lhs.GetDay() < rhs.GetDay()) {
                return true;
            }
            else return false;
        }
        else return false;
    }
    else return false;
}

class Database {
public:
    void AddEvent(const Date& date, const string& event) {
        Calendar[date].insert(event);
    }

    bool DeleteEvent(const Date& date, const string& event) {
        if (Calendar.find(date) != Calendar.end()) {
            if (Calendar.at(date).find(event) != Calendar.at(date).end()) {
                Calendar.at(date).erase(event);
                return true;
            }
            else return false;
            }
        else return false;
    }

    int DeleteDate(const Date& date) {
        int size = 0;
        if (Calendar.find(date) != Calendar.end()) {
            size = Calendar.at(date).size();
            Calendar.erase(date);
        }
        return size;
    }

    void Find(const Date& date) const {
        if (Calendar.find(date) != Calendar.end()) {
            for (const auto& event : Calendar.at(date)) {
                cout << event << endl;
            }
        }
    }

    void Print() const {
        cout << setfill('0');
        string events;
        for (const auto& date : Calendar) {
            for (const auto& event : Calendar.at(date.first)) {
                if (date.first.GetYear() < 0) {
                    cout << '-';
                }
                cout << setw(4) << abs(date.first.GetYear()) << "-" << setw(2) << date.first.GetMonth() << "-" << setw(2) << date.first.GetDay();
                cout << " " << event << endl;
            }
            events.clear();
        }
    }

private:
    map<Date, set<string>> Calendar;
};

int main() {
    Database db;

    string commandString, event = "";
    string dateString;
    vector<string> parsedCommand;
    while (getline(cin, commandString)) {
        dateString = ParseCommand(commandString).at(1);
        event = ParseCommand(commandString).at(2);
        commandString = ParseCommand(commandString).at(0);
        try {
            if (commandString == "Add") {
                db.AddEvent(ConvertToDate(dateString), event);
            }
            else if (commandString == "Del") {
                if (!event.empty()) {
                    if (db.DeleteEvent(ConvertToDate(dateString), event)) {
                        cout << "Deleted successfully" << endl;
                    }
                    else {
                        cout << "Event not found" << endl;
                    }
                }
                else {
                    int deletedEvents = db.DeleteDate(ConvertToDate(dateString));
                    cout << "Deleted " << deletedEvents << " events" << endl;
                }
            }
            else if (commandString == "Find") {
                db.Find(ConvertToDate(dateString));
            }
            else if (commandString == "Print") {
                db.Print();
            }
            else if (commandString == "") {}
            else {
                cout << "Unknown command: " << commandString << endl;
                //exit(0);
            }
        }
        catch (const invalid_argument& ex){
            cout << ex.what() << endl;
            //exit(0);
        }
    }

    return 0;
}
