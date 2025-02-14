#include <cstdint>
#include "../inc/register.hpp"
#include "../inc/instructions.hpp"
#include "../inc/cpu.hpp"
#include "gtest/gtest.h"
#include "../inc/memory.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

/*void from_json(const json& j) {
    auto initialData = j.at("initial");
    Register8(initialData.at("A"),"A");
}*/
/*  struct state {
        std::string name;
        struct initial {
            std::string pc;
            std::string sp;
            std::string a;
            std::string f;
            std::string b;
            std::string c;
            std::string d;
            std::string e;
            std::string h;
            std::string l;
            std::array<std::array<int,2>,65536> ram;
        };

        struct final {
            std::string pc;
            std::string sp;
            std::string a;
            std::string f;
            std::string b;
            std::string c;
            std::string d;
            std::string e;
            std::string h;
            std::string l;
            std::array<std::array<int,3>,65536> cycles;
        };
    };

    void from_json(const json& j, state& s) {
        j.at("name").get_to(s.name);
        j.at("initial").get_to(s.initial);
    }*/
   
void run(std::string  filePath ) {
    std::ifstream f(filePath);
    json data = json::parse(f);
    std::vector<std::array<int, 2>> inputRam;
    std::vector<std::array<int, 2>> outputRam;

    for (auto it = data.begin(); it != data.end(); ++it)
    {
        std::string name = it.value()["name"];
        std::cout << " --- Test " << name << "---" << std::endl;
        //std::cout << *it << std::endl;
        Register8 A((uint8_t) it.value()["initial"]["a"],"A");
        Register8 F((uint8_t) it.value()["initial"]["f"],"F");
        //Register16 AF(&A,&F);
        Register8 B((uint8_t) it.value()["initial"]["b"],"B");
        Register8 C((uint8_t) it.value()["initial"]["c"],"C");
        //Register16 BC(&B,&C);
        Register8 D((uint8_t) it.value()["initial"]["d"],"D");
        Register8 E((uint8_t) it.value()["initial"]["e"],"E");
        //Register16 DE(&D,&E);
        Register8 H((uint8_t) it.value()["initial"]["h"],"H");
        Register8 L((uint8_t) it.value()["initial"]["l"],"L");
        //Register16 HL(&H,&L);
        Register16 SP((uint16_t) it.value()["initial"]["sp"],"SP");
        Register16 PC((uint16_t) it.value()["initial"]["pc"],"PC");
        set_registers(A, F, B, C, D, E, H, L, SP, PC);
        inputRam = it.value()["initial"]["ram"].template get<std::vector<std::array<int, 2>>>();
        /*for (auto ramIt = inputRam.begin(); ramIt != inputRam.end(); ++ramIt) {
            write_byte(ramIt[0],ramIt[1]);
        }*/
        init_mem();
        for (long unsigned int i = 0; i < inputRam.size(); i++) {
            write_byte(inputRam[i][0],(uint8_t) inputRam[i][1]);
        }
        //int running = true;
        //cpu_step(running);
        auto opcode = read_byte(PC.getData_());
        int cycles = 0;
        eval_opcode(opcode, cycles);
        
        EXPECT_EQ(get_register_data8("A"), (uint8_t) it.value()["final"]["a"]);
        EXPECT_EQ(get_register_data8("B"), (uint8_t) it.value()["final"]["b"]);
        EXPECT_EQ(get_register_data8("C"), (uint8_t) it.value()["final"]["c"]);
        EXPECT_EQ(get_register_data8("D"), (uint8_t) it.value()["final"]["d"]);
        EXPECT_EQ(get_register_data8("E"), (uint8_t) it.value()["final"]["e"]);
        EXPECT_EQ(get_register_data8("H"), (uint8_t) it.value()["final"]["h"]);
        EXPECT_EQ(get_register_data8("L"), (uint8_t) it.value()["final"]["l"]);
        EXPECT_EQ(get_register_data8("F"), (uint8_t) it.value()["final"]["f"]);


        EXPECT_EQ(get_register_data16("SP"), (uint16_t) it.value()["final"]["sp"]);
        EXPECT_EQ(get_register_data16("PC"), it.value()["final"]["pc"]);

        outputRam = it.value()["final"]["ram"].template get<std::vector<std::array<int, 2>>>();
        for (long unsigned int i = 0; i < outputRam.size(); i++) {
            std::cout << "Checking ram at address: " << std::dec << outputRam[i][0] << std::endl;
            EXPECT_EQ(read_byte(outputRam[i][0]),(uint8_t) outputRam[i][1]);
        }
        inputRam.clear();
        outputRam.clear();
        free_memory();
        f.close();
    } 
}

std::array<std::string, 500> testFiles = {"00.json","01.json","02.json","03.json","04.json","05.json","06.json","07.json","08.json","09.json","0a.json","0b.json","0c.json","0d.json","0e.json","0f.json","10.json","11.json","12.json","13.json","14.json","15.json","16.json","17.json","18.json","19.json","1a.json","1b.json","1c.json","1d.json","1e.json","1f.json","20.json","21.json","22.json","23.json","24.json","25.json","26.json","27.json","28.json","29.json","2a.json","2b.json","2c.json","2d.json","2e.json","2f.json","30.json","31.json","32.json","33.json","34.json","35.json","36.json","37.json","38.json","39.json","3a.json","3b.json","3c.json","3d.json","3e.json","3f.json","40.json","41.json","42.json","43.json","44.json","45.json","46.json","47.json","48.json","49.json","4a.json","4b.json","4c.json","4d.json","4e.json","4f.json","50.json","51.json","52.json","53.json","54.json","55.json","56.json","57.json","58.json","59.json","5a.json","5b.json","5c.json","5d.json","5e.json","5f.json","60.json","61.json","62.json","63.json","64.json","65.json","66.json","67.json","68.json","69.json","6a.json","6b.json","6c.json","6d.json","6e.json","6f.json","70.json","71.json","72.json","73.json","74.json","75.json","76.json","77.json","78.json","79.json","7a.json","7b.json","7c.json","7d.json","7e.json","7f.json","80.json","81.json","82.json","83.json","84.json","85.json","86.json","87.json","88.json","89.json","8a.json","8b.json","8c.json","8d.json","8e.json","8f.json","90.json","91.json","92.json","93.json","94.json","95.json","96.json","97.json","98.json","99.json","9a.json","9b.json","9c.json","9d.json","9e.json","9f.json","a0.json","a1.json","a2.json","a3.json","a4.json","a5.json","a6.json","a7.json","a8.json","a9.json","aa.json","ab.json","ac.json","ad.json","ae.json","af.json","b0.json","b1.json","b2.json","b3.json","b4.json","b5.json","b6.json","b7.json","b8.json","b9.json","ba.json","bb.json","bc.json","bd.json","be.json","bf.json","c0.json","c1.json","c2.json","c3.json","c4.json","c5.json","c6.json","c7.json","c8.json","c9.json","ca.json","cb 00.json","cb 01.json","cb 02.json","cb 03.json","cb 04.json","cb 05.json","cb 06.json","cb 07.json","cb 08.json","cb 09.json","cb 0a.json","cb 0b.json","cb 0c.json","cb 0d.json","cb 0e.json","cb 0f.json","cb 10.json","cb 11.json","cb 12.json","cb 13.json","cb 14.json","cb 15.json","cb 16.json","cb 17.json","cb 18.json","cb 19.json","cb 1a.json","cb 1b.json","cb 1c.json","cb 1d.json","cb 1e.json","cb 1f.json","cb 20.json","cb 21.json","cb 22.json","cb 23.json","cb 24.json","cb 25.json","cb 26.json","cb 27.json","cb 28.json","cb 29.json","cb 2a.json","cb 2b.json","cb 2c.json","cb 2d.json","cb 2e.json","cb 2f.json","cb 30.json","cb 31.json","cb 32.json","cb 33.json","cb 34.json","cb 35.json","cb 36.json","cb 37.json","cb 38.json","cb 39.json","cb 3a.json","cb 3b.json","cb 3c.json","cb 3d.json","cb 3e.json","cb 3f.json","cb 40.json","cb 41.json","cb 42.json","cb 43.json","cb 44.json","cb 45.json","cb 46.json","cb 47.json","cb 48.json","cb 49.json","cb 4a.json","cb 4b.json","cb 4c.json","cb 4d.json","cb 4e.json","cb 4f.json","cb 50.json","cb 51.json","cb 52.json","cb 53.json","cb 54.json","cb 55.json","cb 56.json","cb 57.json","cb 58.json","cb 59.json","cb 5a.json","cb 5b.json","cb 5c.json","cb 5d.json","cb 5e.json","cb 5f.json","cb 60.json","cb 61.json","cb 62.json","cb 63.json","cb 64.json","cb 65.json","cb 66.json","cb 67.json","cb 68.json","cb 69.json","cb 6a.json","cb 6b.json","cb 6c.json","cb 6d.json","cb 6e.json","cb 6f.json","cb 70.json","cb 71.json","cb 72.json","cb 73.json","cb 74.json","cb 75.json","cb 76.json","cb 77.json","cb 78.json","cb 79.json","cb 7a.json","cb 7b.json","cb 7c.json","cb 7d.json","cb 7e.json","cb 7f.json","cb 80.json","cb 81.json","cb 82.json","cb 83.json","cb 84.json","cb 85.json","cb 86.json","cb 87.json","cb 88.json","cb 89.json","cb 8a.json","cb 8b.json","cb 8c.json","cb 8d.json","cb 8e.json","cb 8f.json","cb 90.json","cb 91.json","cb 92.json","cb 93.json","cb 94.json","cb 95.json","cb 96.json","cb 97.json","cb 98.json","cb 99.json","cb 9a.json","cb 9b.json","cb 9c.json","cb 9d.json","cb 9e.json","cb 9f.json","cb a0.json","cb a1.json","cb a2.json","cb a3.json","cb a4.json","cb a5.json","cb a6.json","cb a7.json","cb a8.json","cb a9.json","cb aa.json","cb ab.json","cb ac.json","cb ad.json","cb ae.json","cb af.json","cb b0.json","cb b1.json","cb b2.json","cb b3.json","cb b4.json","cb b5.json","cb b6.json","cb b7.json","cb b8.json","cb b9.json","cb ba.json","cb bb.json","cb bc.json","cb bd.json","cb be.json","cb bf.json","cb c0.json","cb c1.json","cb c2.json","cb c3.json","cb c4.json","cb c5.json","cb c6.json","cb c7.json","cb c8.json","cb c9.json","cb ca.json","cb cb.json","cb cc.json","cb cd.json","cb ce.json","cb cf.json","cb d0.json","cb d1.json","cb d2.json","cb d3.json","cb d4.json","cb d5.json","cb d6.json","cb d7.json","cb d8.json","cb d9.json","cb da.json","cb db.json","cb dc.json","cb dd.json","cb de.json","cb df.json","cb e0.json","cb e1.json","cb e2.json","cb e3.json","cb e4.json","cb e5.json","cb e6.json","cb e7.json","cb e8.json","cb e9.json","cb ea.json","cb eb.json","cb ec.json","cb ed.json","cb ee.json","cb ef.json","cb f0.json","cb f1.json","cb f2.json","cb f3.json","cb f4.json","cb f5.json","cb f6.json","cb f7.json","cb f8.json","cb f9.json","cb fa.json","cb fb.json","cb fc.json","cb fd.json","cb fe.json","cb ff.json","cc.json","cd.json","ce.json","cf.json","d0.json","d1.json","d2.json","d4.json","d5.json","d6.json","d7.json","d8.json","d9.json","da.json","dc.json","de.json","df.json","e0.json","e1.json","e2.json","e5.json","e6.json","e7.json","e8.json","e9.json","ea.json","ee.json","ef.json","f0.json","f1.json","f2.json","f3.json","f5.json","f6.json","f7.json","f8.json","f9.json","fa.json","fb.json","fe.json","ff.json"};
//std::array<std::string, 499> testFiles = {"00.json","01.json","02.json","03.json","04.json","05.json","06.json","07.json","08.json","09.json","0a.json","0b.json","0c.json","0d.json","0e.json","0f.json","10.json","11.json","12.json","13.json","14.json","15.json","16.json","17.json","18.json","19.json","1a.json","1b.json","1c.json","1d.json","1e.json","1f.json","20.json","21.json","22.json","23.json","24.json","25.json","26.json","27.json","28.json","29.json","2a.json","2b.json","2c.json","2d.json","2e.json","2f.json","30.json","31.json","32.json","33.json","34.json","35.json","36.json","37.json","38.json","39.json","3a.json","3b.json","3c.json","3d.json","3e.json","3f.json","40.json","41.json","42.json","43.json","44.json","45.json","46.json","47.json","48.json","49.json","4a.json","4b.json","4c.json","4d.json","4e.json","4f.json","50.json","51.json","52.json","53.json","54.json","55.json","56.json","57.json","58.json","59.json","5a.json","5b.json","5c.json","5d.json","5e.json","5f.json","60.json","61.json","62.json","63.json","64.json","65.json","66.json","67.json","68.json","69.json","6a.json","6b.json","6c.json","6d.json","6e.json","6f.json","70.json","71.json","72.json","73.json","74.json","75.json","76.json","77.json","78.json","79.json","7a.json","7b.json","7c.json","7d.json","7e.json","7f.json","80.json","81.json","82.json","83.json","84.json","85.json","86.json","87.json","88.json","89.json","8a.json","8b.json","8c.json","8d.json","8e.json","8f.json","90.json","91.json","92.json","93.json","94.json","95.json","96.json","97.json","98.json","99.json","9a.json","9b.json","9c.json","9d.json","9e.json","9f.json","a0.json","a1.json","a2.json","a3.json","a4.json","a5.json","a6.json","a7.json","a8.json","a9.json","aa.json","ab.json","ac.json","ad.json","ae.json","af.json","b0.json","b1.json","b2.json","b3.json","b4.json","b5.json","b6.json","b7.json","b8.json","b9.json","ba.json","bb.json","bc.json","bd.json","be.json","bf.json","c0.json","c1.json","c2.json","c3.json","c4.json","c5.json","c6.json","c7.json","c8.json","ca.json","cb 00.json","cb 01.json","cb 02.json","cb 03.json","cb 04.json","cb 05.json","cb 06.json","cb 07.json","cb 08.json","cb 09.json","cb 0a.json","cb 0b.json","cb 0c.json","cb 0d.json","cb 0e.json","cb 0f.json","cb 10.json","cb 11.json","cb 12.json","cb 13.json","cb 14.json","cb 15.json","cb 16.json","cb 17.json","cb 18.json","cb 19.json","cb 1a.json","cb 1b.json","cb 1c.json","cb 1d.json","cb 1e.json","cb 1f.json","cb 20.json","cb 21.json","cb 22.json","cb 23.json","cb 24.json","cb 25.json","cb 26.json","cb 27.json","cb 28.json","cb 29.json","cb 2a.json","cb 2b.json","cb 2c.json","cb 2d.json","cb 2e.json","cb 2f.json","cb 30.json","cb 31.json","cb 32.json","cb 33.json","cb 34.json","cb 35.json","cb 36.json","cb 37.json","cb 38.json","cb 39.json","cb 3a.json","cb 3b.json","cb 3c.json","cb 3d.json","cb 3e.json","cb 3f.json","cb 40.json","cb 41.json","cb 42.json","cb 43.json","cb 44.json","cb 45.json","cb 46.json","cb 47.json","cb 48.json","cb 49.json","cb 4a.json","cb 4b.json","cb 4c.json","cb 4d.json","cb 4e.json","cb 4f.json","cb 50.json","cb 51.json","cb 52.json","cb 53.json","cb 54.json","cb 55.json","cb 56.json","cb 57.json","cb 58.json","cb 59.json","cb 5a.json","cb 5b.json","cb 5c.json","cb 5d.json","cb 5e.json","cb 5f.json","cb 60.json","cb 61.json","cb 62.json","cb 63.json","cb 64.json","cb 65.json","cb 66.json","cb 67.json","cb 68.json","cb 69.json","cb 6a.json","cb 6b.json","cb 6c.json","cb 6d.json","cb 6e.json","cb 6f.json","cb 70.json","cb 71.json","cb 72.json","cb 73.json","cb 74.json","cb 75.json","cb 76.json","cb 77.json","cb 78.json","cb 79.json","cb 7a.json","cb 7b.json","cb 7c.json","cb 7d.json","cb 7e.json","cb 7f.json","cb 80.json","cb 81.json","cb 82.json","cb 83.json","cb 84.json","cb 85.json","cb 86.json","cb 87.json","cb 88.json","cb 89.json","cb 8a.json","cb 8b.json","cb 8c.json","cb 8d.json","cb 8e.json","cb 8f.json","cb 90.json","cb 91.json","cb 92.json","cb 93.json","cb 94.json","cb 95.json","cb 96.json","cb 97.json","cb 98.json","cb 99.json","cb 9a.json","cb 9b.json","cb 9c.json","cb 9d.json","cb 9e.json","cb 9f.json","cb a0.json","cb a1.json","cb a2.json","cb a3.json","cb a4.json","cb a5.json","cb a6.json","cb a7.json","cb a8.json","cb a9.json","cb aa.json","cb ab.json","cb ac.json","cb ad.json","cb ae.json","cb af.json","cb b0.json","cb b1.json","cb b2.json","cb b3.json","cb b4.json","cb b5.json","cb b6.json","cb b7.json","cb b8.json","cb b9.json","cb ba.json","cb bb.json","cb bc.json","cb bd.json","cb be.json","cb bf.json","cb c0.json","cb c1.json","cb c2.json","cb c3.json","cb c4.json","cb c5.json","cb c6.json","cb c7.json","cb c8.json","cb c9.json","cb ca.json","cb cb.json","cb cc.json","cb cd.json","cb ce.json","cb cf.json","cb d0.json","cb d1.json","cb d2.json","cb d3.json","cb d4.json","cb d5.json","cb d6.json","cb d7.json","cb d8.json","cb d9.json","cb da.json","cb db.json","cb dc.json","cb dd.json","cb de.json","cb df.json","cb e0.json","cb e1.json","cb e2.json","cb e3.json","cb e4.json","cb e5.json","cb e6.json","cb e7.json","cb e8.json","cb e9.json","cb ea.json","cb eb.json","cb ec.json","cb ed.json","cb ee.json","cb ef.json","cb f0.json","cb f1.json","cb f2.json","cb f3.json","cb f4.json","cb f5.json","cb f6.json","cb f7.json","cb f8.json","cb f9.json","cb fa.json","cb fb.json","cb fc.json","cb fd.json","cb fe.json","cb ff.json","cc.json","cd.json","ce.json","cf.json","d0.json","d1.json","d2.json","d4.json","d5.json","d6.json","d7.json","d8.json","d9.json","da.json","dc.json","de.json","df.json","e0.json","e1.json","e2.json","e5.json","e6.json","e7.json","e8.json","e9.json","ea.json","ee.json","ef.json","f0.json","f1.json","f2.json","f3.json","f5.json","f6.json","f7.json","f8.json","f9.json","fa.json","fb.json","fe.json","ff.json"};

TEST (AllInstructionsTest, All) {
    bool allSuits = true;
    //allSuits = false;
    
    if (allSuits) {
        for (long unsigned int i = 0; i < testFiles.size(); i++) {
            std::string filePath = "../sm83/v1/" + testFiles[i];
            std::cout << "=== Testing file " << filePath << " i: "<< i << " ===" << std::endl;
            run(filePath);
        }
    } else {
        
        //int testFileIndex = 204;
        //std::string filePath = "../sm83/v1/" + testFiles[testFileIndex];
        std::string filePath = "../sm83/v1/c9.json";
        std::cout << "=== Testing file " << filePath << " ===" << std::endl;
        run(filePath);     
        std::cout << "HEJ : " << testFiles.size() << std::endl;
    }
}