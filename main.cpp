#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>

int primes[4] = {2, 3, 5, 7};
char suits[4] = {'c', 'd', 'h', 's'};
bool forbiddenCombos[1326];
const int maxMetaclassNumber = 100;
std::unordered_map<int, int> smallestClassMember;
int suitGroups[4];

std::string OneCardNumberToString(int CardNumber) {
    int iRank = CardNumber / 4;
    int iSuit = CardNumber % 4;
    char cRank, cSuit;
    switch (iRank) {
        case 0:
            cRank = '2';
            break;
        case 1:
            cRank = '3';
            break;
        case 2:
            cRank = '4';
            break;
        case 3:
            cRank = '5';
            break;
        case 4:
            cRank = '6';
            break;
        case 5:
            cRank = '7';
            break;
        case 6:
            cRank = '8';
            break;
        case 7:
            cRank = '9';
            break;
        case 8:
            cRank = 'T';
            break;
        case 9:
            cRank = 'J';
            break;
        case 10:
            cRank = 'Q';
            break;
        case 11:
            cRank = 'K';
            break;
        case 12:
            cRank = 'A';
            break;
    }
    switch (iSuit) {
        case 0:
            cSuit = 'c';
            break;
        case 1:
            cSuit = 'd';
            break;
        case 2:
            cSuit = 'h';
            break;
        case 3:
            cSuit = 's';
            break;
    }
    std::string sCard = "xx\0";
    sCard[0] = cRank;
    sCard[1] = cSuit;
    return sCard;
}

int OneCardToNumber(const char Rank, const char Suit) {
    int iRank;
    int iSuit;
    switch (Rank) {
        case '2':
            iRank = 0;
            break;
        case '3':
            iRank = 1;
            break;
        case '4':
            iRank = 2;
            break;
        case '5':
            iRank = 3;
            break;
        case '6':
            iRank = 4;
            break;
        case '7':
            iRank = 5;
            break;
        case '8':
            iRank = 6;
            break;
        case '9':
            iRank = 7;
            break;
        case 'T':
            iRank = 8;
            break;
        case 'J':
            iRank = 9;
            break;
        case 'Q':
            iRank = 10;
            break;
        case 'K':
            iRank = 11;
            break;
        case 'A':
            iRank = 12;
            break;
    }
    switch (Suit) {
        case 'c':
            iSuit = 0;
            break;
        case 'd':
            iSuit = 1;
            break;
        case 'h':
            iSuit = 2;
            break;
        case 's':
            iSuit = 3;
            break;
    }
    return iRank * 4 + iSuit;
}

std::vector<int> getAbsentSuits(std::vector<int> table) {
    int suitFrequency[4] = {0};
    std::vector<int> result;
    for (auto it: table) {
        suitFrequency[it % 4]++;
    }
    for (int i = 0; i < 4; i++) {
        if (!suitFrequency[i])
            result.push_back(i);
    }
    return result;
}

std::vector<int> getPresentSuits(std::vector<int> table) {
    int suitFrequency[4] = {0};
    std::vector<int> result;
    for (auto it: table) {
        suitFrequency[it % 4]++;
    }
    for (int i = 0; i < 4; i++) {
        if (suitFrequency[i])
            result.push_back(i);
    }
    return result;
}

std::vector<int> getSuitsWithEqualRank(std::vector<int> table) {
    std::vector<int> result;
    std::vector<int> suitBuckets[4] = {std::vector<int>(0)};
    std::vector<int> rankBuckets[12] = {std::vector<int>(0)};
    for (auto it: table) {
        suitBuckets[it % 4].push_back(it);
        rankBuckets[it / 4].push_back(it);
    }
    for (auto it: table) {
        if ((rankBuckets[it / 4].size() > 1) && (suitBuckets[it % 4].size() < 2))
            result.push_back(it % 4);
    }
    return result;
}

std::vector<std::vector<int> > getInterchangeableSuits(std::vector<int> table) {
    std::vector<std::vector<int> > result;
    std::vector<int> absentSuits = getAbsentSuits(table);
    std::vector<int> equalRankSuits = getSuitsWithEqualRank(table);
    std::vector<int> presentSuits = getPresentSuits(table);
    std::vector<std::vector<int> > notInterchangeableSuits;
    for (auto suit: presentSuits) {
        if (std::find(equalRankSuits.begin(), equalRankSuits.end(), suit) == equalRankSuits.end())
            notInterchangeableSuits.emplace_back(std::vector<int>(1, suit));
    }
    result = notInterchangeableSuits;
    if (!absentSuits.empty())
        result.push_back(absentSuits);
    if (!equalRankSuits.empty())
        result.push_back(equalRankSuits);
    return result;
}

std::vector<int> readTable(std::string input) {
    std::vector<int> result;
    for (int i = 0; i < input.length() / 3; i++) {
        result.push_back(OneCardToNumber(input[i * 3], input[i * 3 + 1]));
    }
    return result;
}

int getMetaclassHash(int metaclass){
    int firstSuit = metaclass / 4;
    int secondSuit = metaclass % 4;
    return suitGroups[firstSuit] * suitGroups[secondSuit] + 49 * (firstSuit == secondSuit) - 1;
}

int getPairEnumeration(int x, int y){
    return ((x + y)*(x + y + 1)/2) + y;
}

std::vector<std::vector<int> > createMetaclasses(std::vector<std::vector<int> > interchangeableGroups) {
    std::vector<std::vector<int> > metaclasses;
    std::unordered_map<int, std::vector<int> > metaclassDictionary;
    for (int i = 0; i < interchangeableGroups.size(); i++) {
        for (auto it: interchangeableGroups[i])
            suitGroups[it] = primes[i];
    }
    for (int i = 0; i < 4; i++) {
        for (int j = i; j < 4; j++) {
            int metaclass = 4 * i + j;
            metaclassDictionary[getMetaclassHash(metaclass)].emplace_back(metaclass);
        }
    }
    for (int i = 0; i < maxMetaclassNumber; i++) {
        if (!metaclassDictionary[i].empty()) {
            metaclasses.push_back(metaclassDictionary[i]);
        }
    }
    return metaclasses;
}

int hashHand(int firstCard, int secondCard){
    int firstSuit = firstCard % 4;
    int secondSuit = secondCard % 4;
    int firstRank = firstCard / 4;
    int secondRank = secondCard / 4;
    int suitHash = getMetaclassHash(firstSuit * 4 + secondSuit);
    return getPairEnumeration(getPairEnumeration(firstRank, secondRank), suitHash);
}

void unpackMetaclassGroup(std::vector<int> group) {
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 13; j++) {
            for (auto metaclass: group) {
                int firstSuit = metaclass / 4;
                int secondSuit = metaclass % 4;
                if (i != j || firstSuit != secondSuit) {
                    int firstCardNumber = 4 * i + firstSuit;
                    int secondCardNumber = 4 * j + secondSuit;
                    if (firstCardNumber < secondCardNumber)
                        firstCardNumber ^= secondCardNumber ^= firstCardNumber ^= secondCardNumber;
                    int comboNumber = (firstCardNumber - 1) * firstCardNumber / 2 + secondCardNumber;
                    if (!forbiddenCombos[comboNumber]) {
                        smallestClassMember[hashHand(firstCardNumber, secondCardNumber)] = comboNumber;
                        break;
                    }
                }
            }
        }
    }
}

void unpackMetaclasses(std::vector<std::vector<int> > metaclasses){
    for (auto metaclassGroup: metaclasses){
        unpackMetaclassGroup(metaclassGroup);
    }
}

int findSmallestClassMember(std::string hand) {
    int firstCard = OneCardToNumber(hand[0], hand[1]);
    int secondCard = OneCardToNumber(hand[3], hand[4]);
    return smallestClassMember[hashHand(firstCard, secondCard)];
}

int main() {
    std::string input = "2c 3c 4h ";
    //std::getline (std::cin, input);
    std::vector<std::vector<int> > interchangeables = getInterchangeableSuits(readTable(input));
    std::vector<std::vector<int> > metaclasses = createMetaclasses(interchangeables);
    std::cout << "\nMetaclass groups:\n";
    for (auto it: metaclasses) {
        for (auto metaclass: it)
            std::cout << suits[metaclass / 4] << ' ' << suits[metaclass % 4] << '\t';
        std::cout << std::endl;
    }
    unpackMetaclasses(metaclasses);
    std::string hand = "2h 2s";
    std::cout << "\nCalculated hand hash:\t" << hashHand(OneCardToNumber(hand[0], hand[1]), OneCardToNumber(hand[3], hand[4])) << std::endl;
    std::cout << "Smallest class member number:\t" << findSmallestClassMember(hand) << std::endl;
    return 0;
}