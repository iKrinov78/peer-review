using namespace std;

enum DBLogLevel {
    NONE,
    INFO,
    DEBUG,
    TRACE,
    WARNING,
    ERROR
};

struct QueryParams {
    string_view db_name;
    string_view name_filter;
    int db_connection_timeout;
    int min_age;
    int max_age;
    DBLogLevel db_log_level = DBLogLevel::NONE;
    bool db_allow_exceptions = false;

    QueryParams& SetDBName(string_view name) {
        this->db_name = name;
        return *this;
    }
    QueryParams& SetFilterName(string_view name) {
        this->name_filter = name;
        return *this;
    }
    QueryParams& SetDBConnectionTimeout(int value) {
        this->db_connection_timeout = value;
        return *this;
    }
    QueryParams& SetMinAge(int value) {
        this->min_age = value;
        return *this;
    }
    QueryParams& SetMaxAge(int value) {
        this->max_age = value;
        return *this;
    }
    QueryParams& SetLogLevel(DBLogLevel value) {
        this->db_log_level = value;
        return *this;
    }
    QueryParams& SetAlowException(bool value) {
        this->db_allow_exceptions = value;
        return *this;
    }
};

QueryParams qp(QueryParams().SetDBName("test"sv).SetFilterName("filter"sv).SetDBConnectionTimeout(100).SetMinAge(18).SetMaxAge(45));

vector<Person> LoadPersons(QueryParams p) {
    DBConnector connector(p.db_allow_exceptions, p.db_log_level);
    DBHandler db;
    if (p.db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(p.db_name, p.db_connection_timeout);
    } else {
        db = connector.Connect(p.db_name, p.db_connection_timeout);
    }
    if (!p.db_allow_exceptions && !db.IsOK()) {
        return {};
    }

    ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << p.min_age << " and "s << p.max_age << " "s
              << "and Name like '%"s << db.Quote(p.name_filter) << "%'"s;
    DBQuery query(query_str.str());

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({move(name), age});
    }
    return persons;
}
