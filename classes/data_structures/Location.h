//
// Created by joaop on 01/03/2025.
//

#ifndef LOCATION_H
#define LOCATION_H

#include <string>
#include <unordered_set>
//usar como intermédio para depois criar vértice



class Location {
public:
    explicit Location();

    explicit Location(std::string name, std::string code, int id, int parking):
        name(name), code(code), id(id), parking(parking) {}

    struct locationHash { //hash function to use in unordered Map
        std::string operator()(const Location & l) const{
            return l.getCode();
        }
    };

    void setName(std::string name) {
        this->name = name;
    }
    std::string getName() const{
        return this->name;
    }
    void setCode(std::string code) {
        this->code = code;
    }
    std::string getCode() const{
        return this->code;
    }
   int getid() const{
        return this->id;
    }

    void setid(int id) {
        this->id = id;
    }
   int hasparking() const{
        return this->parking;
    }
    void setparking(int parking) {
        this->parking = parking;
    }

    typedef std::unordered_set<Location,Location::locationHash> LocationSet;

private:
  std::string name;
    std::string code;
    int id;
    int parking;

};




#endif //LOCATION_H
