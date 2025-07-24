struct CitiParams {
    vector<City>& cities;
    const Json& json;
    const string& country_name;
    const string& country_iso_code;
    const string& country_phone_code;
    const string& country_time_zone;
    const vector<Language>& languages;

    CitiParams& SetCities(vector<City>& cities) {
        this->cities = std::move(cities);
        return *this;
    }
    CitiParams& SetJson(const Json& json) {
        this->json = std::move(json);
        return *this;
    }
    CitiParams& SetCountry(Country& country) {
        this->country_name = country.name;
        this->country_iso_code = country.iso_code;
        this->country_phone_code = country.phone_code;
        this->country_time_zone = country.time_zone;
        this->languages = std::move(country.languages);
        return *this;
    } 
};

// Дана функция ParseCitySubjson, обрабатывающая JSON-объект со списком городов конкретной страны:
void ParseCitySubjson(const CitiParams& sp) {
    for (const auto& city_json : sp.json.AsList()) {
        const auto& city_obj = city_json.AsObject();
        sp.cities.push_back({city_obj["name"s].AsString(), city_obj["iso_code"s].AsString(),
                          sp.country_phone_code + city_obj["phone_code"s].AsString(), sp.country_name, sp.country_iso_code,
                          sp.country_time_zone, sp.languages});
    }
}

// ParseCitySubjson вызывается только из функции ParseCountryJson следующим образом:
void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {

    for (const auto& country_json : json.AsList()) {
        const auto& country_obj = country_json.AsObject();
        countries.push_back({
            country_obj["name"s].AsString(),
            country_obj["iso_code"s].AsString(),
            country_obj["phone_code"s].AsString(),
            country_obj["time_zone"s].AsString(),
        });
        Country& country = countries.back();
        for (const auto& lang_obj : country_obj["languages"s].AsList()) {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }
        CitiParams sp(CitiParams().SetCities(cities).SetJson(json).SetCountry(country));
        ParseCitySubjson(sp);
    }
}
