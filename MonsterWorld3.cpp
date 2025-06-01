#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cstdio>

using namespace std;

enum WarriorType { Dragon, Ninja, Iceman, Lion, Wolf };
enum WeaponType { Sword, Bomb, Arrow };
enum HQtype { Red, Blue };
string warriorNames[] = { "dragon", "ninja", "iceman", "lion", "wolf" };
string weaponNames[] = { "sword", "bomb", "arrow" };
string HQNames[] = { "red", "blue" };

int initialLife[5], initialAttack[5];
int M, N, R, K, T;

class Warrior;
class HeadQuarter;

class TimeKeeper {
private:
    int hour;
    int minute;

public:
    TimeKeeper() : hour(0), minute(0) {}
    void setHour(int h) {
        hour = h;
    }

    void setMin(int m) {
        minute = m;
    }

    void print() {
        cout << setw(3) << setfill('0') << hour << ":" << setw(2) << setfill('0') << minute << " ";
    }

    void reset() {
        hour = 0;
        minute = 0;
    }
} tk;

class Weapon {
private:
    WeaponType type;
public:
    Weapon(WeaponType type) : type(type) {}
    virtual ~Weapon() {};

    virtual void use() = 0;

    virtual bool outOfUse() = 0;

    virtual int getForce() const = 0;

    virtual int getDurability() = 0;

    WeaponType getType() {
        return type;
    }
};

class sword : public Weapon {
private:
    int force;

public:
    sword(int force) : Weapon(Sword), force(force) {}

    virtual void use() {
        force = force * 4 / 5;
    }

    virtual bool outOfUse() {
        return force <= 0;
    }

    virtual int getForce() const override { return force; }

    virtual int getDurability() {
        return 0;
    }

};

class arrow : public Weapon {
private:
    int durability;
    int r;

public:
    arrow(int r) : Weapon(Arrow), durability(3), r(r) {}
    arrow(int du, int r) : Weapon(Arrow), durability(du), r(r) {}
    virtual void use() {
        durability--;
    }

    virtual bool outOfUse() {
        return durability == 0;
    }

    virtual int getForce() const override { return r; }

    virtual int getDurability() { return durability; }
};

class bomb : public Weapon {
private:
    bool used;
public:
    bomb() : Weapon(Bomb), used(false) {}
    virtual void use() {
        used = true;
    }

    virtual bool outOfUse() {
        return used;
    }

    virtual int getForce() const override { return 0; }

    virtual int getDurability() { return 0; }
};

class Warrior {
protected:
    int life; //生命值
    int power; // 攻击力
    int id; //编号
    WarriorType type; //是什么武士
    HQtype belonging; //属于哪个阵营
    int city; //在第几个城市
    vector<Weapon*> weapons; // 武器

    Weapon* getWeapon(WeaponType type) {
        switch(type) {
            case Sword:
                if (power / 5) return new sword(power / 5);
                else return nullptr;
                break;
            case Arrow:
                return new arrow(R);
                break;
            case Bomb:
                return new bomb();
                break;
        }
        return nullptr;
    }

public:
    bool earnElements;
    bool gotHQ;
    Warrior(int life, int power, int id, WarriorType type, HQtype be, int city) :
    life(life), power(power), id(id), type(type), belonging(be), city(city), earnElements(false), gotHQ(false) {}

    virtual ~Warrior() {
        for(auto i : weapons) {delete i;} 
    }

    virtual bool checkEscape() {
        return false;
    }
    
    virtual void march() {
        if (belonging == Red && city < N + 1) city++;
        if (belonging == Blue && city > 0) city--; 
    }

    virtual void captureWeapons(Warrior * w) {
        w->getId();
    }

    virtual void cheer() {}

    virtual void win() {}

    virtual void lose() {}

    bool gotHeadquarter() { return gotHQ; }

    void earn() {
        life += 8;
        earnElements = false;
    }

    int getCity() {
        return city;
    }

    string getName() {
        return warriorNames[type];
    }

    int getId() {
        return id;
    }

    int getLife() {
        return life;
    }

    int getPower() {
        return power;
    }

    WarriorType getType() {
        return type;
    }

    HQtype getBelonging() {
        return belonging;
    }

    bool hasWeapon(WeaponType type) {
        for(auto i : weapons) {
            if (i->getType() == type) {
                return true;
            }
        }
        return false;
    }

    Weapon* get_Weapon(WeaponType type) {
        for(auto i : weapons) {
            if (i->getType() == type) {
                return i;
            }
        }
        return nullptr;
    }

    void useWeapon(WeaponType type) {
        for(auto i : weapons) {
            if (i->getType() == type) {
                i->use();
                if (i->outOfUse()) {
                    clearWeapon(i);
                }
            }
        }
    }

    void addWeapon(Weapon *w) {
        Weapon* nw;
        switch(w->getType()) {
            case Sword:
            nw = new sword(w->getForce());
            break;
            case Arrow:
            nw = new arrow(w->getDurability(), w->getForce());
            break;
            case Bomb:
            nw = new bomb();
            break;
        }
        weapons.push_back(nw);
    }

    void clearWeapon(Weapon* w) {
        for(auto i = weapons.begin(); i != weapons.end(); i++) {
            if (*i == w) {
                delete *i;
                i = weapons.erase(i);
                break;
            }
        }
    }

    vector<Weapon*> & takeWeapons() {
        return weapons;
    }

    void takeDamage(int damage) {
        life -= damage;
        if (life < 0) life = 0;
    }

    void addWeapons(Weapon* w) {
        weapons.push_back(w);
    }

    void printWeapons() {
        if (weapons.empty()) {
            cout << " has no weapon";
        } else {
            cout << " has ";
            Weapon* w;
            bool first = true;
            w = get_Weapon(Arrow);
            if (w) {
                if (!first) cout << ",";
                cout << "arrow(" << w->getDurability() << ")";
                first = false;
            }
            w = get_Weapon(Bomb);
            if (w) {
                if (!first) cout << ",";
                cout << "bomb";
                first = false;
            }
            w = get_Weapon(Sword);
            if (w) {
                if (!first) cout << ",";
                cout << "sword(" << w->getForce() << ")";
                first = false;
            } 
        }
    }
};

class dragon : public Warrior {
private:
    double morale;

public:
    dragon(int life, int power, int id, HQtype be, double morale, int city) : 
    Warrior(life, power, id, Dragon, be, city), morale(morale) {
        Weapon* w = getWeapon(WeaponType(id % 3));
        if (w) weapons.push_back(w);
    }

    virtual ~dragon() {}

    virtual void cheer() {
        if (morale > 0.8) {
            tk.print();
            cout << HQNames[belonging] << " dragon " << id << " yelled in city " << city << endl;
        }
    }

    virtual void win() {
        morale += 0.2;
    }

    virtual void lose() {
        morale -= 0.2;
    }
};

class ninja : public Warrior {
public:
    ninja(int life, int power, int id, HQtype be, int city) :
    Warrior(life, power, id, Ninja, be, city) {
        Weapon* w = getWeapon(WeaponType(id % 3));
        if (w) weapons.push_back(w);
        Weapon* ww = getWeapon(WeaponType((id + 1) % 3));
        if (ww) weapons.push_back(ww);
    }
    virtual ~ninja() {}

    void fightBack() {}
};

class iceman : public Warrior {
private:
    int step;

public:
    iceman(int life, int power, int id, HQtype be, int city) :
    Warrior(life, power, id, Iceman, be, city), step(0) {
        Weapon* w = getWeapon(WeaponType(id % 3));
        if (w) weapons.push_back(w);
    }
    virtual ~iceman() {}

    virtual void march() override {
        step++;
        if (step % 2 == 0) {
            if (life > 9) life -= 9;
            else life = 1;
            power += 20;
        }
        Warrior::march();
    }
};

class lion : public Warrior {
private:
    int loyalty;

public:
    lion(int life, int power, int id, HQtype be, int city, int loyalty) :
    Warrior(life, power, id, Lion, be, city), loyalty(loyalty) {}
    virtual ~lion() {}

    virtual bool checkEscape() {
        return loyalty <= 0 && (city != (belonging == Red ? N + 1 : 0));
    }

    virtual void lose() {
        loyalty -= K;
    }
};

class wolf : public Warrior {
public:
    wolf(int life, int power, int id, HQtype be, int city) :
    Warrior(life, power, id, Wolf, be, city) {}
    virtual ~wolf() {}

    virtual void captureWeapons(Warrior* enemy) {
        for(auto w : enemy->takeWeapons()) {
            if (!hasWeapon(w->getType())) {
                addWeapon(w);
            } 
            enemy->clearWeapon(w);
        }
    }
};

class City {
private:
    bool hasFlag; // 有没有插旗 
    int id; // 第几个城市
    Warrior* redWarrior; //红方士兵
    Warrior* blueWarrior; //蓝方士兵
    HQtype flagColor; // 旗帜颜色
    HQtype lastWinner; // 上次获胜方
    int elements;
    int consecutiveWins;

public:
    static int totalId;
    bool inil;
    City() : hasFlag(false), id(totalId), redWarrior(nullptr), blueWarrior(nullptr), elements(0), consecutiveWins(0), inil(false) {totalId++;}
    ~City() {}
    
    bool hasBothWarriors() {
        return redWarrior && blueWarrior;
    }

    Warrior* getRedWarrior() {return redWarrior;}
    Warrior* getBlueWarrior() {return blueWarrior;}

    void clearWarrior() {
        redWarrior = nullptr;
        blueWarrior = nullptr;
    }

    void clearWarrior(HQtype type) {
        if (type == Red) redWarrior = nullptr;
        else if (type == Blue) blueWarrior = nullptr;
    }

    void push(Warrior* w, HQtype type) {
        if (type == Red) redWarrior = w;
        else blueWarrior = w;
    }

    void checkLionEscape(Warrior* & Redw, Warrior* & Bluew) {
        if (redWarrior && redWarrior->checkEscape()) {
            Redw = redWarrior;
            tk.print();
            cout << "red lion " << Redw->getId() << " ran away" << endl;
            redWarrior = nullptr;
        }

        if (blueWarrior && blueWarrior->checkEscape()) {
            Bluew = blueWarrior;
            tk.print();
            cout << "blue lion " << Bluew->getId() << " ran away" << endl;
            blueWarrior = nullptr;
        }
    }

    void printWarriorMarch(Warrior* & w, HQtype & type) {
        if (id > 0 && id < N + 1) {
            if (redWarrior) {
                tk.print();
                cout << "red" << " " << redWarrior->getName() << " " << redWarrior->getId() << " marched to city " << id << " with " << redWarrior->getLife() << " elements and force " << redWarrior->getPower() << endl;
            }
            if (blueWarrior) {
                tk.print();
                cout << "blue" << " " << blueWarrior->getName() << " " << blueWarrior->getId() << " marched to city " << id << " with " << blueWarrior->getLife() << " elements and force " << blueWarrior->getPower() << endl;
            }
        } else if (id == 0) {
            if (blueWarrior && !blueWarrior->gotHeadquarter()) {
                tk.print();
                cout << "blue" << " " << blueWarrior->getName() << " " << blueWarrior->getId() << " reached red headquarter with " << blueWarrior->getLife() << " elements and force " << blueWarrior->getPower() << endl;
                w = blueWarrior;
                blueWarrior->gotHQ = true;
                type = Red;
            }
        } else if (id == N + 1) {
            if (redWarrior && !redWarrior->gotHeadquarter()) {
                tk.print();
                cout << "red" << " " << redWarrior->getName() << " " << redWarrior->getId() << " reached blue headquarter with " << redWarrior->getLife() << " elements and force " << redWarrior->getPower() << endl;
                w = redWarrior;
                redWarrior->gotHQ = true;
                type = Blue;
            }
        }
    }

    void addElements(int e) { elements += e; }

    int getElements() const { return elements; }

    void clearElements() { elements = 0; }

    HQtype getAttacker() {
        if (hasFlag) {
            return flagColor;
        } else {
            return (id % 2 == 1) ? Red : Blue;
        }
    }

    void handleBattle(HeadQuarter* redHQ, HeadQuarter* blueHQ);
};

class HeadQuarter {
private:
    int life; //生命元
    vector<WarriorType> productionOrder; //生产顺序
    HQtype type; //红或蓝
    int nextWarrior; //下一个要造的士兵
    int beginCity; //大本营
    int id; // 一共有几个士兵
    int istaken; //是否被占领
    vector<Warrior*> warriors; //士兵

public:
    int toGetLife;
    HeadQuarter(int life, const vector<WarriorType> & order, HQtype type, int begin) : 
    life(life), productionOrder(order), type(type), nextWarrior(0), beginCity(begin), id(0), istaken(0), toGetLife(0) {}

    ~HeadQuarter() {
        for(auto i : warriors) {
            delete i;
        }
    }

    Warrior* getFirstWarrior() {
        if (warriors.empty()) return nullptr;
        else return warriors[warriors.size() - 1];
    }

    void getToGetLife() {
        life += toGetLife;
        toGetLife = 0;
    }

    void produce() {
        WarriorType name = productionOrder[nextWarrior];
        int cost = initialLife[name];
        if (life < cost) return;
        else {
            id++;
            life -= cost;
            Warrior * w;
            tk.print();
            cout << HQNames[type] << " " << warriorNames[name] << " " << id << " born" << endl;
            
            switch(name) {
                case Dragon:
                    w = new dragon(cost, initialAttack[Dragon], id, type, life * 1.0 / cost, beginCity);
                    cout << "Its morale is " << fixed << setprecision(2) << life * 1.0 / cost << endl;
                    break;
                case Ninja:
                    w = new ninja(cost, initialAttack[Ninja], id, type, beginCity);
                    break;
                case Iceman:
                    w = new iceman(cost, initialAttack[Iceman], id, type, beginCity);
                    break;
                case Lion:
                    w = new lion(cost, initialAttack[Lion], id, type, beginCity, life);
                    cout << "Its loyalty is " << life << endl;
                    break;
                case Wolf:
                    w = new wolf(cost, initialAttack[Wolf], id, type, beginCity);
                    break;
            }
            warriors.push_back(w);
            nextWarrior = (nextWarrior + 1) % 5;
        }   
    }

    void WarriorMarch(vector<City*> & city) {
        for(auto i : warriors) {
            i->march();
            city[i->getCity()]->push(i, type);
        }
    }

    void clearWarrior(Warrior * w) {
        for(auto i = warriors.begin(); i != warriors.end(); i++) {
            if (*i == w) {
                delete *i;
                i = warriors.erase(i);
                break;
            }
        }
    }

    void taken() {
        istaken++;
    }

    bool check() {
        return (istaken == 2);
    }

    void earnElements(int l) { life += l; }

    int & getLife() { return life; }

    void reportLife() {
        tk.print();
        cout << life << " elements in " << HQNames[type] << " headquarter" << endl;
    }
};

class War {
private:
    int TIME;
    bool stop;

public:
    HeadQuarter * redHQ;
    HeadQuarter * blueHQ;
    vector<City*> city;
    
    War(int k) : TIME(0), stop(false) {
        cout << "Case " << k << ":" << endl;
        cin >> M >> N >> R >> K >> T;
        for(int i = 0; i < 5; i++) {
            cin >> initialLife[i];
        }
        for(int i = 0; i < 5; i++) {
            cin >> initialAttack[i];
        }
        redHQ = new HeadQuarter(M, {Iceman, Lion, Wolf, Ninja, Dragon}, Red, 0);
        blueHQ = new HeadQuarter(M, {Lion, Dragon, Ninja, Iceman, Wolf}, Blue, N + 1);
        City::totalId = 0;
        for(int i = 0; i < N + 2; i++) {
            city.push_back(new City);
        }
        tk.reset();

        while(true) {
            tk.setHour(TIME);
            // 武士降生00
            if (checkTime(0)) break;
            tk.setMin(0);
            redHQ->produce();
            blueHQ->produce();

            // lion逃跑05
            if (checkTime(5)) break;
            tk.setMin(5);
            checkLionEscape();
            
            // 武士前进10
            if (checkTime(10)) break;
            tk.setMin(10);
            warriorMarch();
            
            if (stop) break;

            // 产出生命元20
            if (checkTime(20)) break;
            tk.setMin(20);
            produceElements();

            //4 获取生命元30
            if (checkTime(30)) break;
            tk.setMin(30);
            getElements();

            //5 武士放箭35
            if (checkTime(35)) break;
            tk.setMin(35);
            warriorShot();

            //6 武士使用炸弹38
            if (checkTime(38)) break;
            tk.setMin(38);
            warriorUseBomb();

            //7 战斗40
            if (checkTime(40)) break;
            tk.setMin(40);
            handleBattles();
        

            //8 司令部报告生命元数量50
            if (checkTime(50)) break;
            tk.setMin(50);
            redHQ->reportLife();
            blueHQ->reportLife();

            //9 武士报告武器情况55
            if (checkTime(55)) break;
            tk.setMin(55);
            reportWeapons();

            TIME++;
        }
    }

    ~War() {
        delete redHQ;
        delete blueHQ;
        for(auto c : city) { delete c;}
    }

    bool checkTime(int t) {
        return (TIME * 60 + t > T);
    }

    void checkLionEscape() {
        Warrior * Rw = redHQ->getFirstWarrior(), * Bw = blueHQ->getFirstWarrior();
        if (Rw && Rw->checkEscape()) {
            tk.print();
            cout << "red lion " << Rw->getId() << " ran away" << endl;
            redHQ->clearWarrior(Rw);
        }
        for(int i = 0; i <= N + 1; i++) {
            Warrior * Redw = nullptr, * Bluew = nullptr;
            city[i]->checkLionEscape(Redw, Bluew);
            if (Redw) redHQ->clearWarrior(Redw);
            if (Bluew) blueHQ->clearWarrior(Bluew);
        }
        if (Bw && Bw->checkEscape()) {
            tk.print();
            cout << "blue lion " << Bw->getId() << " ran away" << endl;
            blueHQ->clearWarrior(Bw);
        }
        
    }

    void warriorMarch() {
        for(int i = 1; i <= N; i++) {
            city[i]->clearWarrior();
        }
        redHQ->WarriorMarch(city);
        blueHQ->WarriorMarch(city);
        for(auto i : city) {
            Warrior* w = nullptr; //占领司令部的士兵
            HQtype type; //可能被占领的司令部类型
            i->printWarriorMarch(w, type);
            if (w) {
                if (type == Red) {
                    // blueHQ->clearWarrior(w);
                    redHQ->taken();
                    if (redHQ->check()) {
                        tk.print();
                        cout << "red headquarter was taken" << endl;
                        stop = true; 
                    }
                } else {
                    // redHQ->clearWarrior(w);
                    blueHQ->taken();
                    if (blueHQ->check()) {
                        tk.print();
                        cout << "blue headquarter was taken" << endl;
                        stop = true;
                    }
                }
            }
        }
    }

    void warriorShot() {
        for (int i = 1; i <= N; ++i) {
            City* c = city[i];
            Warrior* red = c->getRedWarrior();
            Warrior* blue = c->getBlueWarrior();

            // 红方放箭
            if (red && red->hasWeapon(Arrow)) {
                int targetCity = i + 1;
                if (targetCity > N) continue;
                City* target = city[targetCity];
                Warrior* enemy = target->getBlueWarrior();
                if (enemy) {
                    red->useWeapon(Arrow);
                    enemy->takeDamage(R);
                    tk.print();
                    cout << HQNames[Red] << " " << red->getName() << " " << red->getId() << " shot";
                    if (enemy->getLife() <= 0) {
                        cout << " and killed " << HQNames[Blue] << " " << enemy->getName() << " " << enemy->getId();
                        // target->clearWarrior(Blue);
                        // blueHQ->clearWarrior(enemy);
                    }
                    cout << endl;
                }
            }

            // 蓝方放箭
            if (blue && blue->hasWeapon(Arrow)) {
                int targetCity = i - 1;
                if (targetCity < 1) continue;
                City* target = city[targetCity];
                Warrior* enemy = target->getRedWarrior();
                if (enemy) {
                    blue->useWeapon(Arrow);
                    enemy->takeDamage(R);
                    tk.print();
                    cout << HQNames[Blue] << " " << blue->getName() << " " << blue->getId() << " shot";
                    if (enemy->getLife() <= 0) {
                        cout << " and killed " << HQNames[Red] << " " << enemy->getName() << " " << enemy->getId();
                        // target->clearWarrior(Red);
                        // redHQ->clearWarrior(enemy);
                    }
                    cout << endl;
                }
            }
        }
    }

    void produceElements() {
        for(auto c : city) {
            c->addElements(10);
        }
    }

    void getElements() {
        for(int i = 1; i <= N; i++) {
            City* c = city[i];
            if (c->getRedWarrior() && !c->getBlueWarrior()) {
                int earned = c->getElements();
                if (earned > 0) {
                    redHQ->earnElements(earned);
                    tk.print();
                    cout << "red " << c->getRedWarrior()->getName() << " " << c->getRedWarrior()->getId() << " earned " << earned << " elements for his headquarter" << endl;
                    c->clearElements();
                }
            } else if (c->getBlueWarrior() && !c->getRedWarrior()) {
                int earned = c->getElements();
                if (earned > 0) {
                    blueHQ->earnElements(earned);
                    tk.print();
                    cout << "blue " << c->getBlueWarrior()->getName() << " " << c->getBlueWarrior()->getId() << " earned " << earned << " elements for his headquarter" << endl;
                    c->clearElements();
                }
            }
        }
    }

    void warriorUseBomb() {
        for (int i = 1; i <= N; i++) {
            City* c = city[i];
            Warrior* red = c->getRedWarrior();
            Warrior* blue = c->getBlueWarrior();
            HQtype attacker = c->getAttacker();
            if (red && blue && red->getLife() > 0 && blue->getLife() > 0) {
            // cout << "attacker = " << HQNames[attacker] << endl;
            // if (blue) cout << warriorNames[blue->getType()] << endl;
            if (attacker == Red && red && red->getLife() > 0 && blue) {
                int swordForce = 0;
                for (auto w : red->takeWeapons()) {
                    if (w->getType() == Sword) {
                        swordForce = w->getForce();
                        break;
                    }
                }
                
                int attackDamage = red->getPower() + swordForce;
                if (attackDamage < blue->getLife()) {
                    if (blue->getType() == Ninja) continue;
                    swordForce = 0;
                    for (auto w : blue->takeWeapons()) {
                        if (w->getType() == Sword) {
                            swordForce = w->getForce();
                            break;
                        }
                    }
                    int defenderDamage = blue->getPower() / 2 + swordForce;
                    int attackerLifeAfterAttack = red->getLife() - defenderDamage;
                    if (attackerLifeAfterAttack <= 0) {                        
                        if (red->hasWeapon(Bomb)) {
                            red->useWeapon(Bomb);
                            tk.print();
                            cout << "red " << red->getName() << " " << red->getId() << " used a bomb and killed blue " << blue->getName() << " " << blue->getId() << endl;
                            c->clearWarrior();
                            redHQ->clearWarrior(red);
                            blueHQ->clearWarrior(blue);
                            continue;
                        }
                    }
                } else {
                    if (blue->hasWeapon(Bomb)) {
                        blue->useWeapon(Bomb);
                        tk.print();
                        cout << "blue " << blue->getName() << " " << blue->getId() << " used a bomb and killed red " << red->getName() << " " << red->getId() << endl;
                        c->clearWarrior();
                        redHQ->clearWarrior(red);
                        blueHQ->clearWarrior(blue);
                        continue;
                    }
                }
            }
            else if (blue && blue->getLife() > 0 && red) {
                int swordForce = 0;
                for (auto w : blue->takeWeapons()) {
                    if (w->getType() == Sword) {
                        swordForce = w->getForce();
                        break;
                    }
                }
                
                int attackDamage = blue->getPower() + swordForce;
                if (attackDamage < red->getLife()) {
                    if (red->getType() == Ninja) continue;
                    swordForce = 0;
                    for(auto w : red->takeWeapons()) {
                        if (w->getType() == Sword) {
                            swordForce = w->getForce();
                            break;
                        }
                    }
                    int defenderDamage = red->getPower() / 2 + swordForce;
                    int attackerLifeAfterAttack = blue->getLife() - defenderDamage;
                    if (attackerLifeAfterAttack <= 0) {
                        if (blue->hasWeapon(Bomb)) {
                            blue->useWeapon(Bomb);
                            tk.print();
                            cout << "blue " << blue->getName() << " " << blue->getId() << " used a bomb and killed red " << red->getName() << " " << red->getId() << endl;
                            c->clearWarrior();
                            redHQ->clearWarrior(red);
                            blueHQ->clearWarrior(blue);
                            continue;
                        }
                    }
                } else if (red->hasWeapon(Bomb)) {
                        red->useWeapon(Bomb);
                        tk.print();
                        cout << "red " << red->getName() << " " << red->getId() << " used a bomb and killed blue " << blue->getName() << " " << blue->getId() << endl;
                        c->clearWarrior();
                        redHQ->clearWarrior(red);
                        blueHQ->clearWarrior(blue);
                        continue;
                }
            }
            }
        }
    }

    void handleBattles() {
        for(int i = 1; i <= N; i++) {
            city[i]->handleBattle(redHQ, blueHQ);
        }

        for(int i = 1; i <= N; i++) {
            Warrior* redWarrior = city[i]->getRedWarrior();
            if (redWarrior && redWarrior->earnElements) {
                if (redHQ->getLife() >= 8) {
                    redHQ->getLife() -= 8;
                    redWarrior->earn();
                } else {
                    redWarrior->earnElements = false;
                }
            }
        }
        for(int i = N; i >= 1; i--) {
            Warrior* blueWarrior = city[i]->getBlueWarrior();
            if (blueWarrior && blueWarrior->earnElements) {
                if (blueHQ->getLife() >= 8) {
                    blueHQ->getLife() -= 8;
                    blueWarrior->earn();
                } else {
                    blueWarrior->earnElements = false;
                }
            }
        }
        redHQ->getToGetLife();
        blueHQ->getToGetLife();
    }

    void reportWeapons() {
        for (int i = 0; i <= N + 1; i++) {
            Warrior* red = city[i]->getRedWarrior();
            if (red) {
                tk.print();
                cout << HQNames[Red] << " " << red->getName() << " " << red->getId();
                red->printWeapons();
                cout << endl;
            }
        }
        for (int i = 0; i <= N + 1; i++) {
            Warrior* blue = city[i]->getBlueWarrior();
            if (blue) {
                tk.print();
                cout << HQNames[Blue] << " " << blue->getName() << " " << blue->getId();
                blue->printWeapons();
                cout << endl;
            }
        }
    }
};

void City::handleBattle(HeadQuarter* redHQ, HeadQuarter* blueHQ) {
    HQtype attackerType = getAttacker();
    Warrior* attacker = (attackerType == Red) ? redWarrior : blueWarrior;
    Warrior* defender = (attackerType == Red) ? blueWarrior : redWarrior;
    HeadQuarter* attackerHQ = (attackerType == Red) ? redHQ : blueHQ;
    HeadQuarter* defenderHQ = (attackerType == Red) ? blueHQ : redHQ;

    if (attacker && attacker->getLife() <= 0 && defender && defender->getLife() <= 0) {
        attackerHQ->clearWarrior(attacker);
        clearWarrior(attackerType);
        defenderHQ->clearWarrior(defender);
        clearWarrior(defender->getBelonging());
        return;
    }
    if (attacker && attacker->getLife() <= 0) {
        if (!defender || defender->getLife() <= 0) {
            attackerHQ->clearWarrior(attacker);
            clearWarrior(attackerType);
            return;
        }
        if (defender->getType() == Wolf) {
            defender->captureWeapons(attacker);
        }
        if (defender->getType() == Dragon) {
            defender->win();
        }
        attackerHQ->clearWarrior(attacker);
        clearWarrior(attackerType);

        defender->earnElements = true;
        if (elements > 0) {
            defenderHQ->toGetLife += elements;
            tk.print();
            cout << HQNames[defender->getBelonging()] << " " << defender->getName() << " " << defender->getId() << " earned " << elements << " elements for his headquarter" << endl;
            elements = 0;
        }
        if (!inil) {
            lastWinner = defender->getBelonging();
            consecutiveWins = 1;
            inil = true;
        }
        else if (lastWinner == defender->getBelonging()) {
            consecutiveWins++;
        } else {
            consecutiveWins = 1;
            lastWinner = defender->getBelonging();
        }
        if (consecutiveWins == 2) {
            HQtype newflagColor;
            if (!hasFlag) {
                hasFlag = true;
                flagColor = attacker->getBelonging();
                newflagColor = defender->getBelonging();
            } else {
                newflagColor = defender->getBelonging();
            }
            if (newflagColor != flagColor) {
                flagColor = newflagColor;
                tk.print();
                cout << HQNames[defender->getBelonging()] << " flag raised in city " << id << endl;
            }
        }
        return;
    } 

    if (defender && defender->getLife() <= 0) {
        if (!attacker || attacker->getLife() <= 0) {
            defenderHQ->clearWarrior(defender);
            clearWarrior(defender->getBelonging());
            return;
        }
        if (attacker->getType() == Wolf) {
            attacker->captureWeapons(defender);
        }
        if (attacker->getType() == Dragon) {
            attacker->win();
            attacker->cheer();
        }
        defenderHQ->clearWarrior(defender);
        clearWarrior(defender->getBelonging());

        attacker->earnElements = true;
        if (elements > 0) {
            attackerHQ->toGetLife += elements;
            tk.print();
            cout << HQNames[attackerType] << " " << attacker->getName() << " " << attacker->getId() << " earned " << elements << " elements for his headquarter" << endl;
            elements = 0;
        }
        if (!inil) {
            lastWinner = attackerType;
            consecutiveWins = 1;
            inil = true;
        } else if (lastWinner == attackerType) {
            consecutiveWins++;
        } else {
            consecutiveWins = 1;
            lastWinner = attackerType;
        }
        if (consecutiveWins == 2) {
            HQtype newflagColor;
            if (!hasFlag) {
                hasFlag = true;
                flagColor = defender->getBelonging();
                newflagColor = attacker->getBelonging();
            } else {
                newflagColor = attacker->getBelonging();
            }
            if (newflagColor != flagColor) {
                flagColor = newflagColor;
                tk.print();
                cout << HQNames[attacker->getBelonging()] << " flag raised in city " << id << endl;
            }
        }
        return;
    }

    if (!hasBothWarriors()) return;
    tk.print();
    cout << HQNames[attackerType] << " " << attacker->getName() << " " << attacker->getId() << " attacked " << HQNames[defender->getBelonging()] << " " << defender->getName() << " " << defender->getId() << " in city " << id << " with " << attacker->getLife() << " elements and force " << attacker->getPower() << endl;
    int swordForce = 0;
    for (auto w : attacker->takeWeapons()) {
        if (w->getType() == Sword) {
            swordForce = w->getForce();
            w->use();
            if (w->outOfUse()) {
                attacker->clearWeapon(w);
            }
            break;
        }
    }
    int attackDamage = attacker->getPower() + swordForce;
    int defenderLifeBeforeBattle = defender->getLife();
    int attackerLifeBeforeBattle = attacker->getLife();
    defender->takeDamage(attackDamage);

    if (defender->getLife() > 0) {
        if (defender->getType() != Ninja) {
            tk.print();
            cout << HQNames[defender->getBelonging()] << " " << defender->getName() << " " << defender->getId() << " fought back against " << HQNames[attackerType] << " " << attacker->getName() << " " << attacker->getId() << " in city " << id << endl;
            swordForce = 0;
            for (auto w : defender->takeWeapons()) {
                if (w->getType() == Sword) {
                    swordForce = w->getForce();
                    w->use();
                    if (w->outOfUse()) {
                        defender->clearWeapon(w);
                    }
                    break;
                }
            }
            int defendDamage = defender->getPower() / 2 + swordForce;
            attacker->takeDamage(defendDamage);
        }
    }

    if (defender->getLife() <= 0) {
        tk.print();
        cout << HQNames[defender->getBelonging()] << " " << defender->getName() << " " << defender->getId() << " was killed in city " << id << endl;
        if (attacker->getType() == Wolf) {
            attacker->captureWeapons(defender);
        }
        if (attacker->getType() == Dragon) {
            attacker->win();
            attacker->cheer();
        }
        if (defender->getType() == Lion) {
            attacker->takeDamage(-defenderLifeBeforeBattle);
        }
        defenderHQ->clearWarrior(defender);
        clearWarrior(defender->getBelonging());

        attacker->earnElements = true;
        if (elements > 0) {
            attackerHQ->toGetLife += elements;
            tk.print();
            cout << HQNames[attackerType] << " " << attacker->getName() << " " << attacker->getId() << " earned " << elements << " elements for his headquarter" << endl;
            elements = 0;
        }
        if (!inil) {
            lastWinner = attackerType;
            consecutiveWins = 1;
            inil = true;
        }
        else if (lastWinner == attackerType) {
            consecutiveWins++;
        } else {
            consecutiveWins = 1;
            lastWinner = attackerType;
        }
        if (consecutiveWins == 2) {
            HQtype newflagColor;
            if (!hasFlag) {
                hasFlag = true;
                flagColor = defender->getBelonging();
                newflagColor = attacker->getBelonging();
            } else {
                newflagColor = attacker->getBelonging();
            }
            if (newflagColor != flagColor) {
                flagColor = newflagColor;
                tk.print();
                cout << HQNames[attacker->getBelonging()] << " flag raised in city " << id << endl;
            }
        }
    } else if (attacker->getLife() <= 0) {
        tk.print();
        cout << HQNames[attackerType] << " " << attacker->getName() << " " << attacker->getId() << " was killed in city " << id << endl;
        if (defender->getType() == Wolf) {
            defender->captureWeapons(attacker);
        }
        if (defender->getType() == Dragon) {
            defender->win();
        }
        if (attacker->getType() == Lion) {
            defender->takeDamage(-attackerLifeBeforeBattle);
        }
        attackerHQ->clearWarrior(attacker);
        clearWarrior(attackerType);

        defender->earnElements = true;
        if (elements > 0) {
            defenderHQ->toGetLife += elements;
            tk.print();
            cout << HQNames[defender->getBelonging()] << " " << defender->getName() << " " << defender->getId() << " earned " << elements << " elements for his headquarter" << endl;
            elements = 0;
        }
        if (!inil) {
            lastWinner = defender->getBelonging();
            consecutiveWins = 1;
            inil = true;
        } else if (lastWinner == defender->getBelonging()) {
            consecutiveWins++;
        } else {
            consecutiveWins = 1;
            lastWinner = defender->getBelonging();
        }
        if (consecutiveWins == 2) {
            // hasFlag = true;
            // flagColor = defender->getBelonging();
            HQtype newflagColor;
            if (!hasFlag) {
                hasFlag = true;
                flagColor = attacker->getBelonging();
                newflagColor = defender->getBelonging();
            } else {
                newflagColor = defender->getBelonging();
            }
            if (newflagColor != flagColor) {
                flagColor = newflagColor;
                tk.print();
                cout << HQNames[defender->getBelonging()] << " flag raised in city " << id << endl;
            }
        }
        return;
    } else {
        attacker->lose();
        defender->lose();
        consecutiveWins = 0;
        if (attacker && attacker->getType() == Dragon) {
            attacker->cheer();
        }
    }
}

int City::totalId = 0;

int main()
{
    int t;
    cin >> t;
    int k = 1;
    while(t--) {
        War war(k++);
    }
    return 0;
}
