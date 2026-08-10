#pragma once
#include <string>

class Character {
private:
    int id;
    std::string name;
    int maxHp;
    int currentHp;
    std::string type;

protected:
    void setCurrentHp(int hp);

public:
    // Constructor khởi tạo
    Character(int id, const std::string& name, int maxHp, const std::string& type);
    
    // Bắt buộc: Virtual Destructor[cite: 1]
    virtual ~Character() = default;

    // Các hàm Getter (Áp dụng Const correctness)[cite: 1]
    int getId() const;
    const std::string& getName() const;
    int getMaxHp() const;
    int getCurrentHp() const;
    const std::string& getType() const;
    bool isAlive() const;

    // Xử lý logic trạng thái
    void takeDamage(int damage);
    virtual void resetSession();

    // Hàm thuần ảo bắt buộc các lớp con phải ghi đè[cite: 1]
    virtual void performAction(Character& target) = 0;
};