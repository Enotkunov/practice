#include <iostream>
#include <array>
#include <ctime> // для time()
#include <cstdlib> // для rand() и srand()
#include <cassert> 


class Card
{
public:
	enum CardSuit
	{
		SUIT_CLUB,
		SUIT_DIAMOND,
		SUIT_HEART,
		SUIT_SPADE,
		MAX_SUITS
	};

	enum CardRank
	{
		RANK_2,
		RANK_3,
		RANK_4,
		RANK_5,
		RANK_6,
		RANK_7,
		RANK_8,
		RANK_9,
		RANK_10,
		RANK_JACK,
		RANK_QUEEN,
		RANK_KING,
		RANK_ACE,
		MAX_RANKS
	};

private:
	CardRank m_rank;
	CardSuit m_suit;

public:
	Card(CardRank rank = MAX_RANKS, CardSuit suit = MAX_SUITS) :
		m_rank(rank), m_suit(suit)
	{
	}
	
	void printCard() const
	{
		switch (m_rank)
		{
			case RANK_2:		std::cout << '2'; break;
			case RANK_3:		std::cout << '3'; break;
			case RANK_4:		std::cout << '4'; break;
			case RANK_5:		std::cout << '5'; break;
			case RANK_6:		std::cout << '6'; break;
			case RANK_7:		std::cout << '7'; break;
			case RANK_8:		std::cout << '8'; break;
			case RANK_9:		std::cout << '9'; break;
			case RANK_10:		std::cout << 'T'; break;
			case RANK_JACK:		std::cout << 'J'; break;
			case RANK_QUEEN:	std::cout << 'Q'; break;
			case RANK_KING:		std::cout << 'K'; break;
			case RANK_ACE:		std::cout << 'A'; break;
		}

		switch (m_suit)
		{
			case SUIT_CLUB:		std::cout << 'C'; break;
			case SUIT_DIAMOND:	std::cout << 'D'; break;
			case SUIT_HEART:	std::cout << 'H'; break;
			case SUIT_SPADE:	std::cout << 'S'; break;
		}
	}

	int getCardValue() const
	{
		switch (m_rank)
		{
			case RANK_2:		return 2;
			case RANK_3:		return 3;
			case RANK_4:		return 4;
			case RANK_5:		return 5;
			case RANK_6:		return 6;
			case RANK_7:		return 7;
			case RANK_8:		return 8;
			case RANK_9:		return 9;
			case RANK_10:		return 10;
			case RANK_JACK:		return 10;
			case RANK_QUEEN:	return 10;
			case RANK_KING:		return 10;
			case RANK_ACE:		return 11;
		}

		return 0;
	}

};

class Deck
{
private:
	std::array<Card, 52> m_deck;
	int m_cardIndex = 0;
	
	// Генерируем случайное число между min и max (включительно).
	// Предполагается, что srand() уже был вызван (для VS).
	int getRandomNumber(int min, int max)
	{
		// используем static, так как это значение нужно вычислить единожды
		static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0); 

		// Равномерно распределяем вычисление значения из нашего диапазона
		return static_cast<int>(rand() * fraction * (max - min + 1) + min);
	}

	void swapCard(Card &a, Card &b)
	{
		Card temp = a;
		a = b;
		b = temp;
	}

public:
	Deck()
	{
		int card = 0;
		for (int suit = 0; suit < Card::MAX_SUITS; ++suit)
		{
			for (int rank = 0; rank < Card::MAX_RANKS; ++rank)
			{
				m_deck[card] = Card(static_cast<Card::CardRank>(rank), static_cast<Card::CardSuit>(suit));
				++card;
			}
		}
	}

	void printDeck() const
	{
		for (const auto &card : m_deck)
		{
			card.printCard();
			std::cout << ' ';
		}

		std::cout << '\n';
	}
	
	void shuffleDeck()
	{
		// Перебираем каждую карту в колоде
		for (int index = 0; index < 52; ++index)
		{
			// Выбираем любую случайную карту
			int swapIndex = getRandomNumber(0, 51);
			// Меняем местами с нашей текущей картой
			swapCard(m_deck[index], m_deck[swapIndex]);
		}

		// начинаем новую раздачу карт
		m_cardIndex = 0;
	}

	Card &dealCard()
	{
		assert(m_cardIndex < 52);
		return m_deck[m_cardIndex++];
	}
};

char getPlayerChoice()
{
	std::cout << "(h) to hit, or (s) to stand: ";
	char choice;
	do
	{
		std::cin >> choice;
	} while (choice != 'h' && choice != 's');

	return choice;
}


enum BlackjackResult
{
	BLACKJACK_PLAYER_WIN,
	BLACKJACK_DEALER_WIN,
	BLACKJACK_PUSH
};

BlackjackResult playBlackjack(Deck &deck)
{
	int playerPoints = 0;
	int dealerPoints = 0;

	// Дилер получает одну карту
	dealerPoints += deck.dealCard().getCardValue();
	std::cout << "The dealer at start is showing: " << dealerPoints << '\n';

	// Игрок получает две карты
	playerPoints += deck.dealCard().getCardValue();
	playerPoints += deck.dealCard().getCardValue();
	std::cout << "At start you have: " << playerPoints << '\n';

	// Прям таки повезло? Ля ты крыса...НООО...
	if (playerPoints == 21)
	{

		if (dealerPoints < 10)
		{
			return BLACKJACK_PLAYER_WIN;
		}
		else
		{
			// Выбирай, 1 к 1 или всё же чистая победа ?
			char choice = getPlayerChoice();
			if (choice == 's')
				return BLACKJACK_PUSH;
		}
	}			

	// Игрок начинает
	while (1)
	{		
		// Если набирается 21 очков, то автоматически прекращается раздача
		if (playerPoints == 21)
			break;

		// Если набирается больше 21 очков, то автоматический проигрыш 
		if (playerPoints > 21)
			return BLACKJACK_DEALER_WIN;

		char choice = getPlayerChoice();
		if (choice == 's')
			break;

		playerPoints += deck.dealCard().getCardValue();
		std::cout << "You have: " << playerPoints << '\n';
	}

	// Если игрок не проиграл (у него не больше 21 очка), тогда дилер получает карты до тех пор, пока у него в сумме будет не меньше 17 очков
	while (dealerPoints < 17)
	{
		dealerPoints += deck.dealCard().getCardValue();
		std::cout << "The dealer now has: " << dealerPoints << '\n';
	}

	// Если у дилера больше 21, то он проиграл, а игрок выиграл
	if (dealerPoints > 21)
		return BLACKJACK_PLAYER_WIN;

	// Выясняется у кого больше очков в пределах допустимого (до 21)
	if (playerPoints > dealerPoints)
		return BLACKJACK_PLAYER_WIN;
	else if (playerPoints < dealerPoints)
		return BLACKJACK_DEALER_WIN;
	else
		return BLACKJACK_PUSH;
}


void startGame()
{

	int winCounter = 0;
	int loseCounter = 0;
	int pushCounter = 0;

	Deck deck;
	deck.shuffleDeck();

	while (true)
	{
		std::cout << "/******************GAME BEGINS******************\n\n";

		switch (playBlackjack(deck))
		{
			case BLACKJACK_PLAYER_WIN:	++winCounter;	std::cout << "Player win!\n\n";		break;
			case BLACKJACK_DEALER_WIN:	++loseCounter;	std::cout << "Player lose.\n\n";	break;
			case BLACKJACK_PUSH:		++pushCounter;	std::cout << "Push 1 to 1.\n\n";	break;
			default:									std::cout << "OOOPSIIIEEEE.\n\n";	break;
		}

		std::cout << "Statistics: Win = " << winCounter << " | Lose = " << loseCounter << " | Push = " << pushCounter << "\n";

		std::cout << "Do you want to continue game (y) or (n)? ";
		char choice;
		do
		{
			std::cin >> choice;
		} while (choice != 'y' && choice != 'n');

		if (choice == 'y')
		{
			system("cls");
			deck.shuffleDeck();
		}
		else
			break;
	}
}


void exercise4()
{
	srand(static_cast<unsigned int>(time(0))); // используем системные часы в качестве стартового значения
	rand(); // пользователям Visual Studio: делаем сброс первого случайного числа

	startGame();
}
