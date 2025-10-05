#include "UNO.h"
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
using namespace std;

UNOGame::UNOGame(int numPlayers) {
    this->numPlayers = numPlayers;
    directionClockwise = true;
    currentPlayer = 0;
    winner = -1;
}

UNOGame::~UNOGame() {}

void UNOGame::initialize() {
    deck.clear();
    discard.clear();
    players.clear();

    vector<string> colors = {"Red", "Green", "Blue", "Yellow"};
    vector<string> values = {"0","1","2","3","4","5","6","7","8","9","Skip","Reverse","Draw Two"};

    for (string color : colors) {
        deck.push_back({color, "0"});
        for (int i = 1; i <= 9; i++) {
            deck.push_back({color, to_string(i)});
            deck.push_back({color, to_string(i)});
        }
        for (int i = 0; i < 2; i++) {
            deck.push_back({color, "Skip"});
            deck.push_back({color, "Reverse"});
            deck.push_back({color, "Draw Two"});
        }
    }

    mt19937 rng(1234);
    shuffle(deck.begin(), deck.end(), rng);

    players.resize(numPlayers);
    for (int i = 0; i < numPlayers; i++) {
        for (int j = 0; j < 7; j++) {
            players[i].push_back(deck.back());
            deck.pop_back();
        }
    }

    discard.push_back(deck.back());
    deck.pop_back();
}

bool UNOGame::isGameOver() const {
    for (int i = 0; i < numPlayers; i++)
        if (players[i].empty())
            return true;
    return false;
}

int UNOGame::getWinner() const {
    for (int i = 0; i < numPlayers; i++)
        if (players[i].empty())
            return i;
    return -1;
}

string UNOGame::getState() const {
    string s = "Player " + to_string(currentPlayer) + "'s turn, Direction: " +
               (directionClockwise ? "Clockwise" : "Counter-clockwise") +
               ", Top: " + discard.back().color + " " + discard.back().value + ", Players cards: ";
    for (int i = 0; i < numPlayers; i++) {
        s += "P" + to_string(i) + ":" + to_string(players[i].size());
        if (i != numPlayers - 1) s += ", ";
    }
    return s;
}

bool UNOGame::canPlay(const Card& c, const Card& top) {
    return c.color == top.color || c.value == top.value;
}

void UNOGame::playTurn() {
    if (isGameOver()) return;

    Card top = discard.back();
    vector<Card>& hand = players[currentPlayer];

    int playIndex = -1;
    for (int i = 0; i < (int)hand.size(); i++) {
        if (hand[i].color == top.color) { playIndex = i; break; }
    }
    if (playIndex == -1) {
        for (int i = 0; i < (int)hand.size(); i++) {
            if (hand[i].value == top.value) { playIndex = i; break; }
        }
    }
    if (playIndex == -1) {
        for (int i = 0; i < (int)hand.size(); i++) {
            if (hand[i].value == "Skip") { playIndex = i; break; }
        }
        if (playIndex == -1)
            for (int i = 0; i < (int)hand.size(); i++) {
                if (hand[i].value == "Reverse") { playIndex = i; break; }
            }
        if (playIndex == -1)
            for (int i = 0; i < (int)hand.size(); i++) {
                if (hand[i].value == "Draw Two") { playIndex = i; break; }
            }
    }

    if (playIndex == -1) {
        if (!deck.empty()) {
            Card drawn = deck.back();
            deck.pop_back();
            if (canPlay(drawn, top)) {
                discard.push_back(drawn);
            } else {
                hand.push_back(drawn);
                advanceTurn(1);
                return;
            }
        } else {
            advanceTurn(1);
            return;
        }
    } else {
        Card played = hand[playIndex];
        discard.push_back(played);
        hand.erase(hand.begin() + playIndex);

        if (hand.size() == 1)
            cout << "Player " << currentPlayer << " says UNO!" << endl;

        if (played.value == "Skip")
            advanceTurn(2);
        else if (played.value == "Reverse") {
            directionClockwise = !directionClockwise;
            advanceTurn(1);
        } else if (played.value == "Draw Two") {
            int next = getNextPlayer();
            for (int i = 0; i < 2 && !deck.empty(); i++) {
                players[next].push_back(deck.back());
                deck.pop_back();
            }
            advanceTurn(2);
        } else {
            advanceTurn(1);
        }
    }
}

void UNOGame::advanceTurn(int step) {
    for (int i = 0; i < step; i++) {
        if (directionClockwise)
            currentPlayer = (currentPlayer + 1) % numPlayers;
        else
            currentPlayer = (currentPlayer - 1 + numPlayers) % numPlayers;
    }
}

int UNOGame::getNextPlayer() const {
    if (directionClockwise)
        return (currentPlayer + 1) % numPlayers;
    else
        return (currentPlayer - 1 + numPlayers) % numPlayers;
}

int main() {
    UNOGame game(2);
    game.initialize();

    cout << game.getState() << endl;
    game.playTurn();
    cout << game.getState() << endl;
    game.playTurn();
    cout << game.getState() << endl;
    game.playTurn();
    cout << game.getState() << endl;

    return 0;
}
