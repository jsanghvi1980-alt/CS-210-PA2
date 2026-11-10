#include "Adventure.h"
#include <iostream>

// ===================== GIVEN HELPERS (do not change) =====================

bool Adventure::nextLine(std::ifstream& in, std::string& line) {
    std::string raw;

    while (std::getline(in, raw)) {
        int start = 0;

        while (start < (int)raw.size() &&
               (raw[start] == ' ' || raw[start] == '\t')) {
            start++;
        }

        line = raw.substr(start);

        if (!line.empty() && line[line.size() - 1] == '\r') {
            line = line.substr(0, line.size() - 1);
        }

        if (!line.empty()) {
            return true;
        }
    }

    return false;
}

bool Adventure::readField(std::ifstream& in,
                          const std::string& keyword,
                          std::string& value) {
    std::string line;

    if (!nextLine(in, line)) {
        std::cout << "Error: expected a " << keyword
                  << " line but the file ended." << std::endl;
        return false;
    }

    std::string prefix = keyword + " ";

    if (line.substr(0, prefix.size()) != prefix) {
        std::cout << "Error: expected a " << keyword
                  << " line but found: " << line << std::endl;
        return false;
    }

    value = line.substr(prefix.size());
    return true;
}

std::string Adventure::extendPath(const std::string& path,
                                  int choiceNumber) const {
    if (path.empty()) {
        return std::to_string(choiceNumber);
    }

    return path + "-" + std::to_string(choiceNumber);
}

// ===================== LOADING =====================

bool Adventure::load(const std::string& fileName) {
    std::ifstream in(fileName);

    if (!in.is_open()) {
        std::cout << "Error: could not open file "
                  << fileName << std::endl;
        return false;
    }

    std::string value;

    if (!readField(in, "START_HP", value)) {
        return false;
    }

    startHp_ = std::stoi(value);

    Scene<Moment>* root = readScene(in, "START");

    if (root == nullptr) {
        return false;
    }

    tree_.setRoot(root);
    return true;
}

Scene<Moment>* Adventure::readScene(std::ifstream& in,
                                    const std::string& label) {
    std::string text;
    std::string hpText;
    std::string lootText;
    std::string item;
    std::string choicesText;

    if (!readField(in, "SCENE", text) ||
        !readField(in, "HP", hpText) ||
        !readField(in, "LOOT", lootText) ||
        !readField(in, "ITEM", item) ||
        !readField(in, "CHOICES", choicesText)) {
        return nullptr;
    }

    int hpChange = std::stoi(hpText);
    int loot = std::stoi(lootText);
    int numChoices = std::stoi(choicesText);

    if (numChoices < 0 ||
        numChoices > Scene<Moment>::MAX_CHOICES) {
        std::cout << "Error: scene \"" << text << "\" has "
                  << numChoices
                  << " choices. The limit is 3."
                  << std::endl;
        return nullptr;
    }

    Scene<Moment>* scene = new Scene<Moment>(
        new Moment(text, label, hpChange, loot, item),
        numChoices
    );

    for (int i = 0; i < numChoices; i++) {
        std::string childLabel;

        if (!readField(in, "LABEL", childLabel)) {
            delete scene;
            return nullptr;
        }

        Scene<Moment>* child = readScene(in, childLabel);

        if (child == nullptr) {
            delete scene;
            return nullptr;
        }

        scene->setChoice(i, child);
    }

    return scene;
}

// ===================== PLAYING =====================

int Adventure::askChoice(int numChoices) {
    while (true) {
        std::cout << "Your choice: ";

        std::string line;

        if (!std::getline(std::cin, line)) {
            return 0;
        }

        bool isNumber = !line.empty();
        int choice = 0;

        for (char character : line) {
            if (character < '0' || character > '9') {
                isNumber = false;
                break;
            }

            choice = choice * 10 + (character - '0');
        }

        if (isNumber &&
            choice >= 1 &&
            choice <= numChoices) {
            return choice;
        }

        std::cout << "Please type a number from 1 to "
                  << numChoices << "." << std::endl;
    }
}

void Adventure::play() {
    if (tree_.isEmpty()) {
        return;
    }

    Scene<Moment>* scene = tree_.getRoot();
    int hp = startHp_;
    int loot = 0;
    List<Item> bag;

    while (scene != nullptr) {
        Moment* moment = scene->getData();

        hp += moment->getHpChange();
        loot += moment->getLoot();

        std::cout << "----------------------------------------"
                  << moment->getText() << std::endl;

        if (moment->hasItem()) {
            Item item(moment->getItem());

            if (bag.search(item)) {
                std::cout << "You already have: "
                          << moment->getItem() << std::endl;
            } else {
                bag.addFront(new Item(moment->getItem()));

                std::cout << "You picked up: "
                          << moment->getItem() << std::endl;
            }
        }

        std::cout << "HP: " << hp
                  << "  Loot: " << loot << std::endl;

        if (hp <= 0) {
            std::cout << "Your HP hit 0. GAME OVER."
                      << std::endl;
            break;
        }

        if (scene->isEnding()) {
            std::cout << "THE END." << std::endl;
            break;
        }

        for (int i = 0; i < scene->getNumChoices(); i++) {
            std::cout << "[" << i + 1 << "] "
                      << scene->getChoice(i)
                             ->getData()
                             ->getLabel()
                      << std::endl;
        }

        int choice = askChoice(scene->getNumChoices());

        if (choice == 0) {
            return;
        }

        scene = scene->getChoice(choice - 1);
    }

    std::cout << "Final HP: " << hp
              << "  Final loot: " << loot << std::endl;

    std::cout << "Inventory (" << bag.size()
              << " items):" << std::endl;

    bag.print();
}

// ===================== ANALYZING =====================

void Adventure::printStats() const {
}

void Adventure::printSpoilers() const {
}

void Adventure::printEndings(Scene<Moment>* scene,
                             const std::string& path) const {
}

void Adventure::greedyRun() const {
}

int Adventure::bestLoot(Scene<Moment>* scene,
                        int hp,
                        const std::string& path,
                        std::string& bestPath) const {
    return -1;
}

void Adventure::optimalRun() const {
}