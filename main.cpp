#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std;

class Domain {
    public:
    
        Domain(const string& domain) {
            string set_dot = "." + domain;
            reverse(set_dot.begin(), set_dot.end());
            domain_ = set_dot;
        }

        bool operator==(const Domain& other) const {
            return std::lexicographical_compare(domain_.begin(), domain_.end(),
                                                other.domain_.begin(), other.domain_.end());
        }

        bool IsSubdomain(const Domain& other) const {
            if(domain_.size() >= other.domain_.size()) {
                if(equal(domain_.begin(), domain_.begin() + other.domain_.size(), other.domain_.begin())) {
                    return true;
                } 
            }
                return false;
        }

        string GetDomain() const {
            return domain_;
        }

    private:

        string domain_ ;
};

class DomainChecker {
    public:

        template<typename Iterator>
        DomainChecker(const Iterator begin, const Iterator end) {

            vector<Domain> domains(begin, end);

            sort(domains.begin(), domains.end(),
                [](const Domain& lhs, const Domain& rhs) 
                {return lhs == rhs;});

            Iterator iter_unique_domains = unique(domains.begin(), domains.end(),
                [](const Domain& lhs, const Domain& rhs) 
                {return rhs.IsSubdomain(lhs);});

            domains.erase(iter_unique_domains, domains.end());

            forbidden_domains_ = domains;
        }

        bool IsForbidden(const Domain& domain) {
            
            vector<Domain>::iterator iter = upper_bound(forbidden_domains_.begin(),
                forbidden_domains_.end(), domain, 
            [](const Domain& lhs, const Domain& rhs) {
                return lhs.GetDomain() < rhs.GetDomain();
            });

            if(iter == forbidden_domains_.begin()) {
                return false;
            }
            
            return domain.IsSubdomain(*(iter - 1));
        }

    private:

        vector<Domain> forbidden_domains_;
};


const vector<Domain> ReadDomains(istream& input, size_t count) {
    vector<Domain> domains;
    string str;

    for(size_t i = 0; i < count; ++i) {
        getline(input, str);
        domains.push_back(str);
    }
    return domains;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}
