
#include <string>
#include <iostream>

class BaseClass {
protected:
    std::string name;
    int x;
    int y;
public:
    BaseClass(std::string name, int x, int y) : name(name), x(x), y(y) {
        
    }
    virtual void printDescription() const {
        std::cout << "Hello I am of type BaseClass and my name is " << name << std::endl;
    }
};  

class ChildClass : public BaseClass {
public:
    void printDescription() const override {
        std::cout << "hello im of type childclass and my name is " << name << std::endl;
    }
    ChildClass(std::string name, int x, int y) : BaseClass(name, x, y) {
        // extra stuff for constructing the child class
    }
};

int main() {
    ChildClass* child = new ChildClass("Ashwin", 3, 4);
    BaseClass* base = new BaseClass("Moosa", 10, 11);

    std::vector<BaseClass*> arr = {child, base};
    for (const BaseClass* ptr : arr) {
        ptr->printDescription();
    }
}