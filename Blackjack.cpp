#include <iostream>
#include <vector>
#include <stdlib.h> 
#include <string>
#include <time.h>

//globla variable to ensure that srand() is only ever called once.
bool randomized = false;

enum Rank { Ace = 1, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King };
enum Suit { Clubs, Diamonds, Hearts, Spades};

class Card{
    Rank rank;
    Suit suit;
    public:
    Card(Rank rank, Suit suit) : rank(rank), suit(suit) {}
    Card() : rank((Rank) -1), suit((Suit) -1) {}

    int getValue(){
        if (this->rank == Queen || this->rank == King || this->rank == Jack){
            return 10;
        }
        else{
            return this->rank;
        }
    }

    bool getNull(){
        if (this->rank == -1 && this->suit == -1){
            return true;
        }
        return false;
    }

    std::string getString(){
        switch (this->rank){
            case Ten:
            return "10";
            case Queen:
            return "Q";
            case King:
            return "K";
            case Jack:
            return "J";
            default:
            return std::string(1, '0' + this->rank);
        }
    }

    char getSuit(){
        switch (this->suit){
            case 0:
            return 'C';
            case 1:
            return 'D';
            case 2:
            return 'H';
            case 3:
            return 'S';
            default:
            return 'N';
        }
    }
    
    void displayCard(){
        std::cout << this->getString();
        std::cout << this->getSuit() << " ";
    }
};

class Hand{
    protected:

    std::vector<Card> list;

    public:

    Hand(){
        //ensures that the random number is only seeded once, thus making it harder to determine the random sequence,
        //as if it were to reseed every shuffle, a cheater could easily time extactly when the shuffle occurs and then
        // figure out the order of the cards, but a cheater has almost no easy way of determining when the deck object is actually initalized.
        if (!randomized){
            srand(time(nullptr));
        }
        list = std::vector<Card>();
    }

    void printFirst(){
        this->list.at(0).displayCard();
        std::cout << "[" << this->list.at(0).getValue() << "]" << std::endl;
    }

    void print(){
        for (Card cards : this->list){
            cards.displayCard();
        }
        std::cout << "[" << this->getTotal() << "]" << std::endl;
    }

    void add(Card card){
        list.push_back(card);
    }

    void clear(){
        list.clear();
    }

    int getTotal(){
        int sum = 0;
        for (Card cards : this->list){
            sum += cards.getValue();
        }
        return sum;
    }
};

class Deck: public Hand {
    public:

    Deck(){
        this->populate();
        this->shuffle();
    }

    void populate(){
        for (int i = 1; i <= King; i++){
            this->add(Card((Rank) i, Clubs));
            this->add(Card((Rank) i, Diamonds));
            this->add(Card((Rank) i, Hearts));
            this->add(Card((Rank) i, Spades));
        }
    }

    void shuffle(){
        std::vector<Card> shuffled = std::vector<Card>();
        int size = this->list.size();
        while (shuffled.size() != size){
            int index = rand() % this->list.size();
            if (!this->list[index].getNull()){
                shuffled.push_back(this->list[index]);
                this->list[index] = Card();
            }
        }
        this->list = shuffled;
    }

    void deal(Hand& hand){
        if (this->list.size() == 0){
            return;
        }
        hand.add(this->list.back());
        this->list.pop_back();
    }
};

class AbstractPlayer{
    protected:

    Hand hand;

    public:

    Hand& getHand(){
        return this->hand;
    }

    AbstractPlayer(){
        hand = Hand();
    }

    void printFirst(){
        this->hand.printFirst();
    }

    virtual bool isDrawing() = 0;

    bool isBusted(){
        if (hand.getTotal() > 21){
            return true;
        }
        return false;
    }

    int getPoints(){
        return this->hand.getTotal();
    }

    void add(Card card){
        this->hand.add(card);
    }

    void print(){
        this->hand.print();
    }

};

class HumanPlayer: public AbstractPlayer{

    public:

    bool isDrawing(){
        std::cout << "Do you want to draw? (Y/N)" << std::endl;
        std::string input;
        std::cin >> input;
        if (input == "Y"){
            return true;
        }
        else if (input == "N"){
            return false;
        }
        else{
            std::cout << "I didn't understand that. Try again." << std::endl;
            return this->isDrawing();
        }
    }

    void announce(){
        if (this->isBusted()){
            std::cout << "You busted." << std::endl;
        }
        else if (this->getPoints() == 21){
            std::cout << "Player Blackjack!" << std::endl;
        }
    }
};

class ComputerPlayer: public AbstractPlayer{

    public:

    bool isDrawing(){
        if (this->hand.getTotal() <= 16){
            return true;
        }
        return false;
    }

};

class BlackJackGame{
    Deck deck;
    ComputerPlayer casino;
    HumanPlayer player;

    friend class HumanPlayer;

    public:

    void play(){

        this->deck = Deck();
        this->casino = ComputerPlayer();
        this->player = HumanPlayer();

        std::cout << "Welcome to the blackjack table! $15 Minimum bet" << std::endl;

        this->deck.deal(this->casino.getHand());
        this->deck.deal(this->casino.getHand());
        std::cout << "Casino: ";
        this->casino.printFirst();

        this->deck.deal(this->player.getHand());
        this->deck.deal(this->player.getHand());
        std::cout << "Player: ";
        this->player.print();

        if (this->casino.getPoints() > 21){
            std::cout << "House busted." << std::endl;
            return;
        }
        else if (this->casino.getPoints() == 21){
            std::cout << "House drew blackjack." << std::endl;
            return;
        }
        this->playerTurn();
            if (this->player.getPoints() > 21){
            return;
        }
        else if (this->player.getPoints() == 21){
            return;
        }
        

    }

    void computerTurn(){
        while (this->casino.isDrawing()){
            this->deck.deal(this->casino.getHand());
        }
        std::cout << "Casino: ";
        this->casino.print();
    }

    void playerTurn(){
        if (!this->player.isBusted() && this->player.getPoints() != 21 && this->player.isDrawing() ){
            this->deck.deal(this->player.getHand());
            std::cout << "Player: ";
            this->player.print();
        }
        this->player.announce();
    }
};

int main(){
    /*
    Card card = Card(Queen, Clubs);
    card.displayCard();
    Hand* hand = new Hand();
    hand->add(card);
    std::cout << hand->getTotal() << std::endl;
    hand->clear();
    std::cout << hand->getTotal() << std::endl; 
    */

   /*
   Deck* deck = new Deck();
   deck->populate();
   deck->print();
   std::cout << deck->getTotal() << std::endl;
   std::cout << "===============" << std::endl;
   deck->shuffle();
   deck->print();
   std::cout << deck->getTotal() << std::endl;
   */
  
    /*
    Deck* deck = new Deck();
    deck->populate();
    deck->shuffle();
    Hand hand = Hand();
    deck->deal(hand);
    hand.print();
    */

    BlackJackGame game;
    // The main loop of the game
    bool playAgain = true;
    char answer = 'y';
    while (playAgain) {
        game.play();
        // Check whether the player would like to play another round
        std::cout << "Would you like another round? (Y/N): ";
        std::cin >> answer;
        std::cout << std::endl << std::endl;
        playAgain = (answer == 'Y' ? true : false);
    }
    std::cout <<"Game over!";

}