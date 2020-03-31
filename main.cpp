#ifndef __PROGTEST__
#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include <functional>
using namespace std;
#endif /* __PROGTEST__ */

class CComponent {
public:
    virtual void Print(ostream & os, string prefix, string last) const = 0;
    virtual ~CComponent(){}
    virtual CComponent * Clone() = 0;
};

//------------------------------------------------------------------------------------------------
class CCPU : public CComponent {
public:
    CCPU(const int cores, const int frequency):c_cores(cores),c_frequency(frequency) {}

    void Print(ostream & os, string prefix, string last) const{
        os << prefix << last << "CPU, " << c_cores << " cores @ " << c_frequency << "MHz" << endl;
    }

    /* cloning method, returns new object made from object from whitch it's being called */
    virtual CCPU * Clone(){
        return new CCPU (*this);
    }

private:
    int c_cores;
    int c_frequency;
};

//------------------------------------------------------------------------------------------------
class CMemory : public CComponent {
public:
    explicit CMemory(int size):c_size(size){}

    void Print(ostream & os, string prefix, string last) const {
        os << prefix << last << "Memory, " << c_size << " MiB" << endl;
    }

    /* cloning method, returns new object made from object from whitch it's being called */
    virtual CMemory * Clone(){
        return new CMemory (*this);
    }

private:
    int c_size;
};

//------------------------------------------------------------------------------------------------
class CDisk : public CComponent{
public:
    enum Type { MAGNETIC, SSD };
    CDisk(Type type,const int size):c_size(size),c_type(type){
        if (c_type == MAGNETIC) {
            c_stype = "HDD";
        } else {
            c_stype = "SSD";
        }
    }
    CDisk(const CDisk & disk){
        c_size = disk.c_size;
        c_type = disk.c_type;
        c_stype = disk.c_stype;
        c_partitions.clear();
        c_partitions = disk.c_partitions;
    }
    ~CDisk(){
        c_partitions.clear();
    }

    CDisk &operator = (const CDisk & disk){

        if (this == &disk){
            return *this;
        }

        c_size = disk.c_size;
        c_type = disk.c_type;
        c_stype = disk.c_stype;
        c_partitions.clear();
        c_partitions = disk.c_partitions;

        return *this;
    }

    /* cloning method, returns new object made from object from whitch it's being called */
    virtual CDisk * Clone(){
        return new CDisk (*this);
    }

    CDisk & AddPartition(const int size,const string & name){
        c_partitions.push_back(make_pair(size, name));
        return *this;
    }
    void Print(ostream & os, string prefix, string last) const;
private:
    int c_size;
    Type c_type;
    string c_stype;
    vector<pair<int, string>> c_partitions;
};

void CDisk::Print(ostream & os, string prefix, string last) const {

    int size = c_partitions.size();
    string prefix_part;

    os << prefix << last << c_stype << ", " << c_size << " GiB" << endl;

    /* checks if current Disk was last in the print */
    if (last == "+-"){
        prefix_part = "| ";
    } else {
        prefix_part = "  ";
    }

    /* writes out partitions */
    for (int i = 0; i < size-1; i++){
        os << prefix << prefix_part << "+-[" << i << "]: " << c_partitions[i].first << " GiB, " << c_partitions[i].second << endl;
    }

    if (size != 0){
        os << prefix << prefix_part << "\\-[" << size-1 << "]: " << c_partitions[size-1].first << " GiB, " << c_partitions[size-1].second << endl;
    }
}

//------------------------------------------------------------------------------------------------
class CComputer {
public:
    explicit CComputer(const string & name): c_name(name){}
    ~CComputer(){
        int size = c_components.size();
        /* frees memory from pointers */
        for (int i = 0; i < size; i++) {
            delete c_components[i];
        }
        c_components.clear();
        c_adresses.clear();
    }
    CComputer(const CComputer & computer){
        int size = computer.c_components.size();
        c_name = computer.c_name;
        c_adresses.clear();
        c_adresses = computer.c_adresses;

        /* clones the object correctly and adds pointer on it to the vector */
        for (int i = 0; i < size; i++){
            CComponent * comp = computer.c_components[i]->Clone();
            c_components.push_back(comp);
        }
    }

    CComputer & operator = (const CComputer & computer){

        if (this == &computer){
            return *this;
        }

        int size_comp = computer.c_components.size();
        int size = c_components.size();

        c_name = computer.c_name;
        c_adresses.clear();
        c_adresses = computer.c_adresses;

        /* frees any objects in the destination CComputer */
        for (int i = 0; i < size; i++){
            delete c_components[i];
        }

        c_components.clear();

        /* clones the object correctly and adds pointer on it to the vector */
        for (int i = 0; i < size_comp; i++){
            CComponent * comp = computer.c_components[i]->Clone();
            c_components.push_back(comp);
        }

        return *this;
    }

    CComputer & AddAddress(const string & address){ c_adresses.push_back(address); return *this; }
    CComputer & AddComponent(const CCPU & cpu) {
        CComponent * component = new CCPU(cpu);
        c_components.push_back(component);
        return *this;
    }
    CComputer & AddComponent(const CDisk & disk){
        CComponent * component = new CDisk(disk);
        c_components.push_back(component);
        return *this;
    }
    CComputer & AddComponent(const CMemory & memory){
        CComponent * component = new CMemory(memory);
        c_components.push_back(component);
        return *this;
    }

    string GetName() const { return c_name; }

    void Print(ostream & os, string prefix, string last) const {
        int size_addr = c_adresses.size();
        int size_comp = c_components.size();

        os << last << "Host: " << c_name << endl;

        for (int i = 0; i < size_addr; i++){
            os << prefix << "+-" << c_adresses[i] << endl;
        }

        for (int n = 0; n < size_comp - 1; n++){
            (*c_components[n]).Print(os, prefix, "+-");
        }

        if (size_comp != 0){
            (*c_components[size_comp-1]).Print(os, prefix, "\\-");
        }
    }
private:
    string c_name;
    vector<string> c_adresses;
    vector<CComponent*> c_components;
};

ostream & operator << (ostream & os, const CComputer & computer) {
    computer.Print(os, "", "");
    return os;
}

//------------------------------------------------------------------------------------------------
class CNetwork {
public:
    explicit CNetwork(const string & name):c_name(name){}
    CNetwork(const CNetwork & network);
    ~CNetwork(){
        int size = c_computer_vector.size();

        /* goes through vector and frees allocated pointers */
        for (int i = 0; i < size; i++){
            delete c_computer_vector[i];
        }
        c_computer_vector.clear();
        c_computers.clear();
    }

    CNetwork & AddComputer(const CComputer & computer);
    CComputer* FindComputer(const string & name) const;

    CNetwork &operator = (const CNetwork & network);

    void Print(ostream & os) const;

private:
    string c_name;

    /* I am using map and vector for storing CComputer objects(or pointers on them). \
     * In each there is a pointer on the same object in memory therefore when I free them in destructor or = operand I only need to go through vector array.(or I think that is right)
     * Map is used for search so that it is fast
     * Vector is for printing them out based on how they were added */
    map<string, CComputer*> c_computers;
    vector<CComputer*> c_computer_vector;
};

CNetwork::CNetwork(const CNetwork &network) {

    c_name = network.c_name;

    int size_comp = network.c_computer_vector.size();

    for (int i = 0; i < size_comp; i++){
        CComputer * comp = new CComputer( (*network.c_computer_vector[i]));
        c_computers.insert( make_pair( network.c_computer_vector[i]->GetName(), comp));
        c_computer_vector.push_back(comp);
    }

}

CNetwork &CNetwork::AddComputer(const CComputer &computer)  {
    CComputer * comp = new CComputer(computer);
    c_computers.insert(make_pair(computer.GetName(), comp ));
    c_computer_vector.push_back(comp);
    return *this;
}

CComputer* CNetwork::FindComputer(const string &name) const {
   auto it = c_computers.find(name);

   if (it == c_computers.end()){
       return nullptr;
   } else {
       return it->second;
   }
}

void CNetwork::Print(ostream &os) const {

    int size = c_computer_vector.size();

    os << "Network: " << c_name << endl;

    for (int i = 0; i < size -1; i++){
        (*c_computer_vector[i]).Print(os, "| ", "+-");
    }

    if (size != 0) {
        (*c_computer_vector[size-1]).Print(os, "  ", "\\-");
    }

}

CNetwork &CNetwork::operator=(const CNetwork &network) {

    if (this == &network){
        return *this;
    }

    c_name = network.c_name;

    int size_net = network.c_computer_vector.size();

    int size = c_computer_vector.size();


    /* frees any objects in the destination CNetwork */
    for (int i = 0; i < size; i++){
        delete c_computer_vector[i];
    }

    c_computers.clear();
    c_computer_vector.clear();

    /* clones new objects into vector and map */
    for (int i = 0; i < size_net; i++){
        CComputer * comp = new CComputer( (*network.c_computer_vector[i]));
        c_computers.insert( make_pair( network.c_computer_vector[i]->GetName(), comp));
        c_computer_vector.push_back(comp);
    }

    return *this;
}

ostream & operator << (ostream & os, const CNetwork & network){
    network.Print(os);
    return os;
}

#ifndef __PROGTEST__
template<typename _T>
string toString ( const _T & x )
{
    ostringstream oss;
    oss << x;
    return oss . str ();
}

int main ( void )
{

    CComputer *p = new CComputer ( "progtest.fit.cvut.cz" );

    (*p).AddAddress ( "147.32.232.142" ) .
            AddComponent ( CCPU ( 8, 2400 ) ) .
            AddComponent ( CCPU ( 8, 1200 ) ) .
            AddComponent ( CDisk ( CDisk::MAGNETIC, 1500 ) .
            AddPartition ( 50, "/" ) .
            AddPartition ( 5, "/boot" ).
            AddPartition ( 1000, "/var" ) ) .
            AddComponent ( CDisk ( CDisk::SSD, 60 ) .
            AddPartition ( 60, "/data" )  ) .
            AddComponent ( CMemory ( 2000 ) ).
            AddComponent ( CMemory ( 2000 ) );

    cout << "////////////" << endl;


    //cout << *p;

    CNetwork n ( "FIT network" );

    n . AddComputer(*p);
    n . AddComputer(*p);
    n . AddComputer(*p);
    n . AddComputer(*p);

    CNetwork x = n;

    cout << n;

    CNetwork n ( "FIT network" );
   n . AddComputer (
           CComputer ( "progtest.fit.cvut.cz" ) .
                   AddAddress ( "147.32.232.142" ) .
                   AddComponent ( CCPU ( 8, 2400 ) ) .
                   AddComponent ( CCPU ( 8, 1200 ) ) .
                   AddComponent ( CDisk ( CDisk::MAGNETIC, 1500 ) .
                   AddPartition ( 50, "/" ) .
                   AddPartition ( 5, "/boot" ).
                   AddPartition ( 1000, "/var" ) ) .
                   AddComponent ( CDisk ( CDisk::SSD, 60 ) .
                   AddPartition ( 60, "/data" )  ) .
                   AddComponent ( CMemory ( 2000 ) ).
                   AddComponent ( CMemory ( 2000 ) ) ) .
           AddComputer (
           CComputer ( "edux.fit.cvut.cz" ) .
                   AddAddress ( "147.32.232.158" ) .
                   AddComponent ( CCPU ( 4, 1600 ) ) .
                   AddComponent ( CMemory ( 4000 ) ).
                   AddComponent ( CDisk ( CDisk::MAGNETIC, 2000 ) .
                   AddPartition ( 100, "/" )   .
                   AddPartition ( 1900, "/data" ) ) ) .
           AddComputer (
           CComputer ( "imap.fit.cvut.cz" ) .
                   AddAddress ( "147.32.232.238" ) .
                   AddComponent ( CCPU ( 4, 2500 ) ) .
                   AddAddress ( "2001:718:2:2901::238" ) .
                   AddComponent ( CMemory ( 8000 ) ) );

     assert ( toString ( n ) ==
              "Network: FIT network\n"
              "+-Host: progtest.fit.cvut.cz\n"
              "| +-147.32.232.142\n"
              "| +-CPU, 8 cores @ 2400MHz\n"
              "| +-CPU, 8 cores @ 1200MHz\n"
              "| +-HDD, 1500 GiB\n"
              "| | +-[0]: 50 GiB, /\n"
              "| | +-[1]: 5 GiB, /boot\n"
              "| | \\-[2]: 1000 GiB, /var\n"
              "| +-SSD, 60 GiB\n"
              "| | \\-[0]: 60 GiB, /data\n"
              "| +-Memory, 2000 MiB\n"
              "| \\-Memory, 2000 MiB\n"
              "+-Host: edux.fit.cvut.cz\n"
              "| +-147.32.232.158\n"
              "| +-CPU, 4 cores @ 1600MHz\n"
              "| +-Memory, 4000 MiB\n"
              "| \\-HDD, 2000 GiB\n"
              "|   +-[0]: 100 GiB, /\n"
              "|   \\-[1]: 1900 GiB, /data\n"
              "\\-Host: imap.fit.cvut.cz\n"
              "  +-147.32.232.238\n"
              "  +-2001:718:2:2901::238\n"
              "  +-CPU, 4 cores @ 2500MHz\n"
              "  \\-Memory, 8000 MiB\n" );

    CNetwork x = n;
    auto c = x . FindComputer ( "imap.fit.cvut.cz" );
    assert ( toString ( *c ) ==
             "Host: imap.fit.cvut.cz\n"
             "+-147.32.232.238\n"
             "+-2001:718:2:2901::238\n"
             "+-CPU, 4 cores @ 2500MHz\n"
             "\\-Memory, 8000 MiB\n" );
    c -> AddComponent ( CDisk ( CDisk::MAGNETIC, 1000 ) .
            AddPartition ( 100, "system" ) .
            AddPartition ( 200, "WWW" ) .
            AddPartition ( 700, "mail" ) );

    assert ( toString ( x ) ==
             "Network: FIT network\n"
             "+-Host: progtest.fit.cvut.cz\n"
             "| +-147.32.232.142\n"
             "| +-CPU, 8 cores @ 2400MHz\n"
             "| +-CPU, 8 cores @ 1200MHz\n"
             "| +-HDD, 1500 GiB\n"
             "| | +-[0]: 50 GiB, /\n"
             "| | +-[1]: 5 GiB, /boot\n"
             "| | \\-[2]: 1000 GiB, /var\n"
             "| +-SSD, 60 GiB\n"
             "| | \\-[0]: 60 GiB, /data\n"
             "| +-Memory, 2000 MiB\n"
             "| \\-Memory, 2000 MiB\n"
             "+-Host: edux.fit.cvut.cz\n"
             "| +-147.32.232.158\n"
             "| +-CPU, 4 cores @ 1600MHz\n"
             "| +-Memory, 4000 MiB\n"
             "| \\-HDD, 2000 GiB\n"
             "|   +-[0]: 100 GiB, /\n"
             "|   \\-[1]: 1900 GiB, /data\n"
             "\\-Host: imap.fit.cvut.cz\n"
             "  +-147.32.232.238\n"
             "  +-2001:718:2:2901::238\n"
             "  +-CPU, 4 cores @ 2500MHz\n"
             "  +-Memory, 8000 MiB\n"
             "  \\-HDD, 1000 GiB\n"
             "    +-[0]: 100 GiB, system\n"
             "    +-[1]: 200 GiB, WWW\n"
             "    \\-[2]: 700 GiB, mail\n" );
    assert ( toString ( n ) ==
             "Network: FIT network\n"
             "+-Host: progtest.fit.cvut.cz\n"
             "| +-147.32.232.142\n"
             "| +-CPU, 8 cores @ 2400MHz\n"
             "| +-CPU, 8 cores @ 1200MHz\n"
             "| +-HDD, 1500 GiB\n"
             "| | +-[0]: 50 GiB, /\n"
             "| | +-[1]: 5 GiB, /boot\n"
             "| | \\-[2]: 1000 GiB, /var\n"
             "| +-SSD, 60 GiB\n"
             "| | \\-[0]: 60 GiB, /data\n"
             "| +-Memory, 2000 MiB\n"
             "| \\-Memory, 2000 MiB\n"
             "+-Host: edux.fit.cvut.cz\n"
             "| +-147.32.232.158\n"
             "| +-CPU, 4 cores @ 1600MHz\n"
             "| +-Memory, 4000 MiB\n"
             "| \\-HDD, 2000 GiB\n"
             "|   +-[0]: 100 GiB, /\n"
             "|   \\-[1]: 1900 GiB, /data\n"
             "\\-Host: imap.fit.cvut.cz\n"
             "  +-147.32.232.238\n"
             "  +-2001:718:2:2901::238\n"
             "  +-CPU, 4 cores @ 2500MHz\n"
             "  \\-Memory, 8000 MiB\n" );
    cout << "test" << endl;

    CNetwork y("POKUS");
    y=n;

    return 0;

}
#endif /* __PROGTEST__ */
