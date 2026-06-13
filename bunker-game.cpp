#include <iostream>
#include <random>

int day = 0;
int hP = 100;
int food = 5;
int water = 5;
int medkit = 1;
int ammunition = 3;
bool is_running = true;

int get_random(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

void clear_screen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

int choice(int num) {
    int choice;
    while (true) {
        std::cout << "Обери дію (1-" << num << "): ";
        if (std::cin >> choice) {
            if (choice >= 1 && choice <= num) {
                break;
            }
        }
        std::cout << "Неправильний ввід! Спробуй ще раз.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    return choice;
}

void wait_for_key() {
    std::cout << "\nНатисніть Enter, щоб продовжити...";
    std::cin.clear();
    std::cin.ignore(10000, '\n');
    std::cin.get(); 
}

void krok4() {
    int rundomNum = get_random(1, 3);
    switch (rundomNum) {
        case 1:
            std::cout << "\n[ПОДІЯ] Знайдено занедбаний магазин! +3 їжі, +3 води\n";
            food += 3;
            water += 3;
            break;
        case 2:
            std::cout << "\n[ПОДІЯ] Напали вороги! \n";
            if (ammunition > 0) {
                ammunition -= 1;
                medkit += 1;
                std::cout << "Ти відстрілявся! Витрачено 1 патрон, знайдено 1 аптечку.\n";
            } else {
                hP -= 40;
                std::cout << "У тебе не було патронів! Ти ледве втік і отримав 40 шкоди.\n";
            }
            break;
        case 3:
            std::cout << "\n[ПОДІЯ] Гравець довго бродив, нічого не знайшов і втратив 10% здоров'я.\n";
            hP -= 10;
            break;
    }
}

void krok3() {
    std::cout << "\nВиберіть дію:\n";
    std::cout << "1. Залишитися в бункері (+15 HP)\n";
    std::cout << "2. Вилазка за ресурсами (Рейд)\n";
    std::cout << "3. Використати аптечку (+50 HP)\n";
    
    while (true) {
        int choicee = choice(3);
        if (choicee == 1) {
            hP = std::min(100, hP + 15);
            std::cout << "Ти відпочив у бункері.\n";
            break;
        } else if (choicee == 2) {
            krok4();
            break;
        } else if (choicee == 3) {
            if (medkit > 0) {
                medkit -= 1;
                hP = std::min(100, hP + 50);
                std::cout << "Ти використав аптечку й підлікувався.\n";
                break;
            } else {
                std::cout << "У тебе немає аптечок! Обери іншу дію.\n";
            }
        }
    }
}

int main() {
    std::cout << "=== ГРА БУНКЕР ===\n";
    wait_for_key();

    while (is_running) {
        day += 1;
        clear_screen();
        std::cout << "=== ДЕНЬ " << day << " ===\n";

        // Споживання ресурсів
        if (food > 0) {
            food -= 1;
        } else {
            hP -= 30;
            std::cout << "[УВАГА] Немає їжі! Твоє здоров'я падає.\n";
        }

        if (water > 0) {
            water -= 1;
        } else {
            hP -= 30;
            std::cout << "[УВАГА] Немає води! Твоє здоров'я падає.\n";
        }

        // Перевірка на смерть від голоду/спраги на початку дня
        if (hP <= 0) {
            std::cout << "\nТи не вижив у цьому суворому світі від виснаження. Гра закінчена.\n";
            is_running = false;
            break;
        }

        // Виведення стану
        std::cout << "Здоров'я: " << hP << "%\n";
        std::cout << "Їжа: " << food << "\nВода: " << water << "\n";
        std::cout << "Аптечки: " << medkit << "\nПатрони: " << ammunition << "\n";

        krok3(); // Хід гравця (вибір дії та можливий рейд)

        // Перевірка на смерть після рейду
        if (hP <= 0) {
            std::cout << "\nТи загинув від отриманих ран. Гра закінчена.\n";
            is_running = false;
            break;
        }

        // Перевірка перемоги
        if (day == 10) {
            std::cout << "\nВітаємо! Рятувальний гелікоптер прибув. Ти вижив усі 10 днів! 🛸\n";
            is_running = false;
            break;
        }

        wait_for_key();
    }
    return 0;
}