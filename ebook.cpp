#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;



class BookStatistic {
    public:

        BookStatistic() : users_progres_(MAX_USERS + 1, 0),
                          pages_statistic_(MAX_PAGES, 0) {}
        void Read(const size_t& user, const size_t& page);
        double Cheer(const size_t& user);

    private:
        static const size_t MAX_USERS = 1e+5;
        static const size_t MAX_PAGES = 1000;
        size_t users_count_ = 0;
        vector<size_t> users_progres_;
        vector<size_t> pages_statistic_;
};

void BookStatistic::Read(const size_t& user, const size_t& page) {
    size_t pages_read = 0;

    if(!users_progres_[user]) {
        ++users_count_;
    } else {
        pages_read = users_progres_[user];
    }

    users_progres_[user] = page;

    for(size_t i = pages_read; i < page; ++i) {
        ++pages_statistic_[i];
    }        
}

double BookStatistic::Cheer(const size_t& user) {
    size_t page = users_progres_[user];
    if(!page) {
        return 0.0;
    } else if(!(users_count_ - 1)) {
        return 1.0;
    }

    return (users_count_ - pages_statistic_[page - 1]) * 1.0 / (users_count_ - 1);
}

int main() {
    BookStatistic statistic;
    
    size_t number_requests{0};
    string request{};
    size_t user_id{};
    size_t page{};
    cin >> number_requests;
    for(size_t i = 0; i < number_requests; ++i) {
        cin >> request;
        if(request == "READ") {
            cin >> user_id >> page;
            statistic.Read(user_id, page);
        } else if(request == "CHEER") {
            cin >> user_id;
            cout << std::setprecision(6) << statistic.Cheer(user_id) << "\n";
        }
    }
}
