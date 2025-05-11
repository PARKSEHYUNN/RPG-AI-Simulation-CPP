#include <iostream>
#include <string>
#include <iomanip>
#include <random>
#include <windows.h>

// RNG 엔진
std::random_device rd;
std::mt19937 rng(time(0));

class Function {
    public:
        static std::string center(const std::string& text, int width) {
            int padding = width - static_cast<int>(text.length());
            if (padding <= 0) return text;

            int padLeft = padding / 2;
            int padRight = padding - padLeft;

            return std::string(padLeft, ' ') + text + std::string(padRight, ' ');
        }

        static std::string repeat(const std::string& text, int count) {
            std::string result = "";

            for(int i = 0; i < count; i ++) {
                result += text;
            }

            return result;
        }

        static float clamp(float value, float min, float max) {
            if(value < min) return min;
            if(value > max) return max;
            return value;
        }
};

class Character {
    protected:
        const std::string NAME; // 캐릭터 객체 이름
        const int MAX_HP, MAX_MP; // 캐릭터 객체 최대 체력, 최대 마나
        const int ATK, MATK, DEF; // 캐릭터 객체 공격력, 마력, 방어력
        const float ACC, AVD; // 캐릭터 객체 명중률, 회피율
        int HP, MP; // 캐릭터 객체 현재 체력, 마력
        bool isDefending = false; // 캐릭터 방어 태세 확인

    public:
        // 캐릭터 객체 기본 생성자
        Character(std::string characterNAME, int characterMaxHP, int characterMaxMP, int characterATK, int characterMATK, int characterDEF, float characterACC, float characterAVD)
        : NAME(characterNAME),
          MAX_HP(characterMaxHP), MAX_MP(characterMaxMP),
          ATK(characterATK), MATK(characterMATK), DEF(characterDEF),
          ACC(characterACC), AVD(characterAVD),
          HP(characterMaxHP), MP(characterMaxMP) {

        }

        // 캐릭터 이름 전달 메소드
        virtual std::string getName() const {
            return NAME;
        }

        // 캐릭터 명중률 전달 메소드
        virtual float getACC() const {
            return ACC;
        }

        // 캐릭터 회피율 전달 메소드
        virtual float getAVD() const {
            return AVD;
        }

        // 캐릭터 방어 상태 확인 메소드
        virtual bool getDefending() const {
            return isDefending;
        }

        // 캐릭터 사망 확인 메소드
        virtual bool isDead() const {
            return HP <= 0;
        }

        // 캐릭터 기본 공격 메소드
        virtual void attack(Character& target) {
            const float HIT_CHANCE = Function::clamp(ACC - target.AVD, 5.0f, 100.0f); // 공격 명중 확률

            // 랜덤 값
            std::uniform_int_distribution<int> dist(0, 99);
            int HIT_RATE = dist(rng);

            // 공격이 빗나갔을때
            if(HIT_CHANCE < HIT_RATE) {
                const std::string MISS_TEXT = "* " + target.NAME + " dodged " + NAME + "'s attack.";
                std::cout << MISS_TEXT << std::endl;
            }
            // 공격이 명중했을때
            else {
                const int EFFECTIVE_DEF = target.DEF * (target.getDefending() ? 2 : 1); // 대상의 방어 상태 확인 후 대상의 방어력 적용
                const int ATTACK_DAMAGE = ATK * (float) (1 - EFFECTIVE_DEF / (float) (EFFECTIVE_DEF + ATK)); // 실제 적용 데미지

                target.HP -= ATTACK_DAMAGE; // 상대의 HP 감소

                const std::string DAMAGE_TEXT = "* " + NAME + " attacked " + target.NAME + " and dealt " + std::to_string(ATTACK_DAMAGE) + " damage.";

                std::cout << DAMAGE_TEXT << std::endl;
            }
        }

        // 캐릭터 방어 태세 전환 메소드
        virtual void defense() {
            isDefending = !isDefending;
        }

        // 캐릭터 행동 선택 메소드
        virtual void action(Character& target) {
            int boxWidth = 21;
            std::string topLine = u8"┌" + Function::repeat("─", boxWidth) + "┐";
            std::string bottomLine = u8"└" + Function::repeat("─", boxWidth) + "┘";

            std::cout << topLine << std::endl;

            std::cout << u8"│" << Function::center(NAME + "`s Turn", boxWidth) << u8"│" << std::endl;
            std::cout << u8"├" << Function::repeat("─", boxWidth) << u8"┤" << std::endl;

            std::cout << u8"│" << Function::center("1. Attack", boxWidth) << "│" << std::endl;
            std::cout << u8"│" << Function::center("2. Use Skill", boxWidth) << "│" << std::endl;
            std::cout << u8"│" << Function::center("3. Defense", boxWidth) << "│" << std::endl;
            std::cout << bottomLine << std::endl;

            char choice;

            while(true) {
                std::cout << "Input: ";
                std::cin >> choice;

                if(std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid input. Please enter 1, 2, or 3.\n";
                    continue;
                }

                if(choice == '1' || choice == '2' || choice == '3') {
                    break;
                }
                else {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid input. Please enter 1, 2, or 3.\n";
                }
            }

            if(choice == '1') { // 기본 공격
                attack(target);
            }
            else if(choice == '2') { // 스킬 사용
                // 스킬
            }
            else if(choice == '3') { // 방어 태세 변환
                defense();
            }
            
        }

        // 캐릭터 정보 출력 메소드
        virtual void printInfo() const {
            int boxWidth = 41;
            std::string topLine = u8"┌" + Function::repeat("─", boxWidth) + "┐";
            std::string bottomLine = u8"└" + Function::repeat("─", boxWidth / 2) + u8"┴" + Function::repeat("─", boxWidth / 2) + "┘";

            std::string printHP = "HP: " + std::to_string(HP) + "/" + std::to_string(MAX_HP);
            std::string printMP = "MP: " + std::to_string(MP) + "/" + std::to_string(MAX_MP);

            std::string printATK = "ATK: " + std::to_string(ATK);
            std::string printMATK = "MATK: " + std::to_string(MATK);
            std::string printDEF = "DEF: " + std::to_string(DEF);

            std::ostringstream ossACC;
            ossACC << std::fixed << std::setprecision(1) << ACC;

            std::ostringstream ossAVD;
            ossAVD << std::fixed << std::setprecision(1) << ACC;

            std::string printACC = "ACC: " + ossACC.str() + "%";
            std::string printAVD = "AVD: " + ossAVD.str() + "%";

            std::cout << topLine << std::endl;
            std::cout << u8"│" << Function::center(NAME, boxWidth) << u8"│" << std::endl; // 캐릭터 이름 출력
            std::cout << u8"├" << Function::repeat("─", boxWidth) << u8"┤" << std::endl;
            std::cout << u8"│" << Function::center(printHP, boxWidth) << u8"│" << std::endl; // 캐릭터 HP 출력
            std::cout << u8"│" << Function::center(printMP, boxWidth) << u8"│" << std::endl; // 캐릭터 MP 출력

            std::cout << u8"├" << Function::repeat("─", boxWidth / 3) << u8"┬" << Function::repeat("─", boxWidth / 3) << u8"┬" << Function::repeat("─", boxWidth / 3) << u8"┤" << std::endl;

            std::cout << u8"│" << Function::center(printATK, boxWidth / 3); // 캐릭터 공격력 출력
            std::cout << u8"│" << Function::center(printMATK, boxWidth / 3); // 캐릭터 마력 출력
            std::cout << u8"│" << Function::center(printDEF, boxWidth / 3) << u8"│" << std::endl; // 캐릭터 마력 출력
            
            std::cout << u8"├" << Function::repeat("─", boxWidth / 3) << u8"┴" << Function::repeat("─", boxWidth / 3 / 2) << u8"┬" << Function::repeat("─", boxWidth / 3 / 2) << u8"┴" << Function::repeat("─", boxWidth / 3) << u8"┤" << std::endl;

            std::cout << u8"│" << Function::center(printACC, boxWidth / 2); // 캐릭터 명중률 출력
            std::cout << u8"│" << Function::center(printAVD, boxWidth / 2) << u8"│" << std::endl; // 캐릭터 회피율 출력

            std::cout << bottomLine << std::endl;
        }

};

int main() {
    // 폰트 설정
    SetConsoleOutputCP(65001);

    while(true) {
        // 콘솔 지우기
        system("cls");

        Character player("Player", 100, 100, 10, 10, 10, 10.0f, 10.0f);
        player.printInfo();

        Character player2("Player2", 100, 100, 10, 10, 10, 10.0f, 10.0f);
        player2.printInfo();

        player.action(player2);

        if(player2.isDead()) {
            std::cout << "* " + player2.getName() + " is Dead!" << std::endl;
            std::cout << "* " + player.getName() + " is Win!" << std::endl;
            break;
        }

        player2.action(player);

        if(player.isDead()) {
            std::cout << "* " + player.getName() + " is Dead!" << std::endl;
            std::cout << "* " + player2.getName() + " is Win!" << std::endl;
            break;
        }
    }

    

    system("pause");
    return 0;
}
