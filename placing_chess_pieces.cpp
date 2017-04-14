#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <vector>
#include <cassert>
#include <boost/functional/hash.hpp>

// Google code jam 2017, Qualification round
// Problem D O(N) solution
// Problem link: https://code.google.com/codejam/contest/3264486/dashboard#s=p3

struct Piece {
	int i, j;           // row and column
	char t;             // piece type
	bool given = false; // is given by input?
	bool operator==(const Piece & o) const {
		return i == o.i && j == o.j;
	}
};

struct PieceHash { // Hash type for Piece type used for unordered_set
	size_t operator()(const Piece & piece) const {
		size_t ret = piece.i;
		boost::hash_combine(ret, piece.j);
		return ret;
	}
};

typedef std::unordered_set<Piece, PieceHash> PieceSet;

// Given row,column in the original grid, return row and column in 45 degree rotated grid
std::pair<int, int> toDiagonal(int N, int i, int j) {
	return {N - 1 + j - i, i + j};
}

// Reverse function of toDiagonal()
std::pair<int, int> fromDiagonal(int N, int i, int j) {
	return {-(i - j - N + 1) / 2, (i + j - N + 1) / 2};
}

bool is_valid_in_rotated_grid(int N, int i, int j) {
	// Move position to the left top corner
	if (i > N-1) i = (2*N-2)-i;
	if (j > N-1) j = (2*N-2)-j;
	// Test
	return i >= (N-1)-j && (i % 2 == j % 2) == N % 2;
	//     ^^^^^^^^^^^^    ^^^^^^^^^^^^^^^^^^^^^^^^^
	//     Corner test        Inside blank test
}

struct Solver {
	const int N;
	int value;
	std::vector<bool> taken_rows, taken_cols; 
	std::vector<bool> taken_rotated_rows, taken_rotated_cols; 
	PieceSet pieces;
	Solver(int N) 
		: N(N)
		, value(0)
		, taken_rows(N, false)
		, taken_cols(N, false)
		, taken_rotated_rows(2*N-1, false)
		, taken_rotated_cols(2*N-1, false) {}

	void set(const Piece & piece) {
		switch(piece.t) {
			case '+':
				// Bishop case
				value++;
				pieces.insert(piece);
				{
				auto ret = toDiagonal(N, piece.i, piece.j);
				assert(is_valid_in_rotated_grid(N, ret.first, ret.second));
				taken_rotated_rows[ret.first] = true;
				taken_rotated_cols[ret.second] = true;
				}
				return;
			case 'x':
				// Rook case
				value++;
				pieces.insert(piece);
				taken_rows[piece.i] = true;
				taken_cols[piece.j] = true;
				return;
			case 'o':
				// Queen case
				pieces.insert(piece);
				set({piece.i, piece.j, '+', true});
				set({piece.i, piece.j, 'x', true});
				return;
		}
		// unreachable
	}

	std::vector<Piece> solve() {
		const auto makeQueen = [this](auto it) {
			auto queen = *it;
			pieces.erase(it);
			queen.t = 'o';
			queen.given = false;
			pieces.insert(queen);
		};
		{
			// Find possible rook's positions
			std::vector<int> not_taken_rows;
			for (int i = 0 ; i < taken_rows.size() ; ++i) {
				if (taken_rows[i]) continue;
				not_taken_rows.push_back(i);
			}
			for (int i = 0 ; i < taken_cols.size(); ++i) {
				if (taken_cols[i]) continue;
				assert(!not_taken_rows.empty());
				value++;
				auto ret = pieces.insert(Piece{not_taken_rows.back(), i, 'x'});
				if (!ret.second) makeQueen(ret.first);
				not_taken_rows.pop_back();
			}
		}
		{
			// Find possible bishops' positions
			// Order the indices from the one with more space to the one with less space
			std::vector<int> index_order(2*N-1);
			int idx = 0;
			index_order[idx++] = N-1; 
			for (int i = N-2 ; i >= 0 ; --i) {
				index_order[idx++] = i;
				index_order[idx++] = (2*N-2)-i;
			}

			std::vector<int> not_taken_odd_rows, not_taken_even_rows;
			for (const auto o: index_order) {
				if (taken_rotated_rows[o]) continue;
				if (o % 2) not_taken_odd_rows.push_back(o);
				else not_taken_even_rows.push_back(o);
			}

			for (const auto o: index_order) {
				if (taken_rotated_cols[o]) continue;
				auto & not_taken_rows = (o%2 == (N%2))?not_taken_odd_rows:not_taken_even_rows;
				while (!not_taken_rows.empty()) {
					int i = not_taken_rows.back();
					not_taken_rows.pop_back();
					if (!is_valid_in_rotated_grid(N, i, o)) continue;
					value++;
					const auto p = fromDiagonal(N, i, o);
					auto ret = pieces.insert(Piece{p.first, p.second, '+'});
					if (!ret.second) makeQueen(ret.first);
					break;
				}
			}
		}

		// Make output
		std::vector<Piece> ret;
		for (const auto & piece : pieces) {
			if (piece.given) continue;
			ret.push_back(piece);
		}
		return ret;
	}
};

int main(int argc, const char *argv[])
{
	int T;
	int N, M;

	std::cin >> T;
	for (int tcase = 1 ; tcase <= T ; ++tcase) {
		std::cout << "Case #" << tcase << ": ";
		std::cin >> N >> M;
		Solver solver(N);
		for (int m = 0 ; m < M ; ++m) {
			Piece c;
			std::cin >> c.t >> c.i >> c.j;
			c.i--, c.j--, c.given = true;
			solver.set(c);
		}
		auto pieces = solver.solve();
		std::cout << solver.value << " " << pieces.size() << std::endl;
		for (const auto & p : pieces) {
			std::cout << p.t << " " << p.i+1 << " " << p.j+1 << std::endl;
		}
	}
	return 0;
}
