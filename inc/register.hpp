#include <cstdint>
#include <unistd.h>
#include <string>

#ifndef REGISTER_H
#define REGISTER_H
class Register8 {
    private:
        uint8_t data;
        std::string name;         

    public:
        uint8_t getData();
        std::string getName();
        void setData(uint8_t data);
        void setName(std::string name);
        //std::string toString(); 
        Register8();        
        Register8(uint8_t data, std::string name);
};

class Register16 { 
    private:
        Register8* low;
        Register8* high;
        uint16_t data;
        std::string name;

    public:
        Register8* getLow();
        uint8_t getLowData();
        /*uint8_t getLowData() {
            return (*low).getData();
        }*/
        Register8* getHigh();
        /*uint8_t getHighData() {
            return (*high).getData();
        }*/
        uint8_t getHighData();
        uint16_t getData();
        uint16_t getData_();
        std::string getName();

        void setLow(Register8* data);
        void setHigh(Register8* data);
        void setData(uint16_t data);

        void setData_(uint16_t data);
        void setName(std::string name);
        void inc(uint16_t n);

        //std::string toString();
        Register16();
        Register16(Register8* low, Register8* high);
        Register16(uint16_t data, std::string name);
};
#endif
