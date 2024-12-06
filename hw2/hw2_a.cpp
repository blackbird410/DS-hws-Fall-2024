#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define STATION_NAMES "station_name.txt"
#define BOOKING_RECORD "booking_record.txt"
#define SUMMARY_FILE "summary.txt"
#define STATION_NAME 1

class BookingRecord;
class BookingSystem;
class SummaryRecord;

class Station {
  int id;
  std::string name;

public:
  Station() {};
  Station(int i, std::string &n) : id(i), name(n) {};
  ~Station() {};

  bool operator!=(Station *other) const { return id != other->id; };

  friend class BookingRecord;
  friend class BookingSystem;
  friend class SummaryRecord;

  friend std::ostream &operator<<(std::ostream &out, const Station *s) {
    out << s->id << " " << s->name;
    return out;
  }
};

class BookingRecord {
  Station *start;
  Station *end;
  int nTickets;

public:
  BookingRecord() {};
  BookingRecord(Station *s, Station *e, int n)
      : start(s), end(e), nTickets(n) {};
  ~BookingRecord() {};

  friend class BookingSystem;

  friend std::ostream &operator<<(std::ostream &out, const BookingRecord *b) {
    if (b)
      out << b->start->id << " " << b->end->id << " " << b->nTickets;
    return out;
  }
};

class SummaryRecord {
  Station *station;
  int nTickets;

public:
  SummaryRecord() {};
  SummaryRecord(int &i, std::string &stationName, int n)
      : station(new Station(i, stationName)), nTickets(n) {};
  SummaryRecord(Station *s, int n) : station(s), nTickets(n) {};
  ~SummaryRecord() {};

  friend class BookingSystem;

  friend std::ostream &operator<<(std::ostream &out, const SummaryRecord *r) {
    if (r)
      out << r->station << " " << r->nTickets;
    return out;
  }
};

class SourceDestinationRecord {
  std::string start;
  std::string end;
  int nPassengers;

public:
  explicit SourceDestinationRecord(std::string &s, std::string &e, int &n)
      : start(s), end(e), nPassengers(n) {};

  friend class BookingSystem;

  friend std::ostream &operator<<(std::ostream &out,
                                  const SourceDestinationRecord *r) {
    if (r)
      out << r->start << " " << r->end << " " << r->nPassengers;
    return out;
  }
};

class BookingSystem {
  std::vector<Station *> stations;
  std::vector<BookingRecord *> records;

public:
  BookingSystem() {};
  ~BookingSystem() {
    for (auto &station : stations)
      delete station;
    for (auto &record : records)
      delete record;

    stations.clear();
    records.clear();
  };

  void insertStation(int &id, std::string &name) {
    stations.push_back(new Station(id, name));
  };

  Station *findStation(int &id) {
    for (auto &station : stations)
      if (station->id == id)
        return station;
    return nullptr;
  };

  void insertRecord(int &startID, int &endID, int &nTickets) {
    if (!nTickets)
      return;

    Station *start = findStation(startID);
    Station *end = findStation(endID);

    if (start && end)
      records.push_back(new BookingRecord(start, end, nTickets));
  };

  void printRecords() {
    for (auto &record : records)
      std::cout << record << std::endl;
  };

  void printStations() {
    for (auto &station : stations)
      std::cout << station << std::endl;
  };

  int getEntries(int &stationID) {
    int entries = 0;
    for (auto &record : records)
      entries += (record->start->id == stationID) ? record->nTickets : 0;
    return entries;
  };

  int getExits(int &stationID) {
    int exits = 0;
    for (auto &record : records)
      exits += (record->end->id == stationID) ? record->nTickets : 0;
    return exits;
  };

  int getSourceDestinationPassengers(int &sourceID, int &destinationID) {
    int sum = 0;
    for (auto &record : records)
      sum += (record->start->id == sourceID && record->end->id == destinationID)
                 ? record->nTickets
                 : 0;
    return sum;
  };

  static bool compName(SummaryRecord *a, SummaryRecord *b) {
    return a->station->name < b->station->name;
  };

  static bool compNTickets(SummaryRecord *a, SummaryRecord *b) {
    return a->nTickets < b->nTickets;
  };

  static bool compSourceDestination(SourceDestinationRecord *a,
                                    SourceDestinationRecord *b) {
    return a->nPassengers < b->nPassengers;
  };

  void writeSummary() {
    std::vector<SummaryRecord *> resultEntries;
    std::vector<SummaryRecord *> resultExits;
    std::vector<SourceDestinationRecord *> nonZeroRecords;
    int tmpCount = 0;

    for (auto &station : stations)
      resultEntries.push_back(
          new SummaryRecord(station, getEntries(station->id)));

    for (auto &station : stations)
      resultExits.push_back(new SummaryRecord(station, getExits(station->id)));

    for (auto &start : stations) {
      for (auto &end : stations) {
        tmpCount = (start != end)
                       ? getSourceDestinationPassengers(start->id, end->id)
                       : 0;
        if (tmpCount)
          nonZeroRecords.push_back(
              new SourceDestinationRecord(start->name, end->name, tmpCount));
      }
    }

    sort(resultEntries.begin(), resultEntries.end(), this->compName);
    sort(resultExits.begin(), resultExits.end(), this->compNTickets);
    sort(nonZeroRecords.begin(), nonZeroRecords.end(),
         this->compSourceDestination);

    std::ofstream outFile(SUMMARY_FILE);
    for (auto &record : resultEntries)
      outFile << record << std::endl;
    outFile << std::endl;

    for (auto &record : resultExits)
      outFile << record << std::endl;
    outFile << std::endl;

    for (auto &record : nonZeroRecords)
      outFile << record << std::endl;

    for (auto &record : resultEntries)
      delete record;
    for (auto &record : resultExits)
      delete record;
    for (auto &record : nonZeroRecords)
      delete record;

    resultEntries.clear();
    resultExits.clear();
    nonZeroRecords.clear();

    std::cout << "Summary written to " << SUMMARY_FILE << std::endl;
  };
};

int main() {
  std::ifstream inFile(STATION_NAMES);
  std::ifstream bookingFile(BOOKING_RECORD);
  std::string line;
  int id;
  std::string name;
  int startID;
  int endID;
  int nTickets;
  BookingSystem system;

  while (inFile >> id >> name) {
    system.insertStation(id, name);
  }

  while (bookingFile >> startID >> endID >> nTickets) {
    system.insertRecord(startID, endID, nTickets);
  }

  system.writeSummary();
  return 0;
}
