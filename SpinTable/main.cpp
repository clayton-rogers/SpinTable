


#include <iostream>
#include <vector>
#include <algorithm>



using List_t = std::vector<int>;


List_t gen_init(int n) {
	List_t list;
	for (int i = 0; i < n; ++i) {
		list.push_back(i);
	}

	return list;
}

void print_list(List_t l) {
	for (const auto& i : l) {
		std::cout << i << " ";
	}
	std::cout << std::endl;
}

struct Result_t {
	int max_number_of_matches = 0;
	int offset = 0;
};

Result_t get_max_number_of_matches(List_t l) {
	const int n = l.size();
	Result_t r;

	// offset represents the number of spins about the table
	for (int offset = 0; offset < n; ++offset) {
		int number_of_matches = 0;
		for (int i = 0; i < n; ++i) {
			const int required_value = (i + offset) % n;
			if (l.at(i) == required_value) {
				++number_of_matches;
			}
		}
		if (number_of_matches > r.max_number_of_matches) {
			r.max_number_of_matches = number_of_matches;
			r.offset = offset;
		}
	}

	return r;
}

List_t get_shifted_list(const List_t& l, const int shift_amount) {
	List_t r;
	const int n = l.size();
	for (int i = 0; i < n; ++i) {
		int index = (n + i - shift_amount) % n;
		r.push_back(l.at(index));
	}

	return r;
}

void print_result(const List_t& l, const Result_t& r) {
	std::cout << "Num matches: " << r.max_number_of_matches << " Shift: " << r.offset << std::endl;
	const int n = l.size();
	const auto shifted_l = get_shifted_list(l, r.offset);
	for (int i = 0; i < n; ++i) {
		std::cout << shifted_l.at(i);
		if (shifted_l.at(i) == i) {
			std::cout << "*";
		} else {
			std::cout << " ";
		}
		std::cout << " ";
	}
	std::cout << std::endl;
}

struct State_t {
	List_t list;
	Result_t result;
};

void do_work(State_t* state, const List_t& l) {
	auto r = get_max_number_of_matches(l);
	// try to find the list with the minimum max number of matches
	if (r.max_number_of_matches < state->result.max_number_of_matches) {
		state->list = l;
		state->result = r;
	}
}

// Heap's algorithm
// see https://en.wikipedia.org/wiki/Heap%27s_algorithm
State_t generate_every_permutation_and_calc(List_t A, void(*fn)(State_t*, const List_t&)) {
	std::vector<List_t> ret;
	const int n = A.size();
	std::vector<int> c(n, 0);

	State_t state;
	state.result.max_number_of_matches = n;
	fn(&state, A);

	int i = 0;
	while (i < n) {
		if (c.at(i) < i) {
			if (i % 2 == 0) {
				std::swap(A.at(0), A.at(i));
			} else {
				std::swap(A.at(c.at(i)), A.at(i));
			}

			fn(&state, A);

			c.at(i) += 1;

			i = 0;
		} else {
			c.at(i) = 0;
			i += 1;
		}
	}

	return state;
}

void test() {
	auto l = gen_init(7);
	print_list(l);
	auto r = get_max_number_of_matches(l);
	print_result(l, r);

	l = List_t{ 1, 2, 3, 4, 0 };
	print_list(l);
	r = get_max_number_of_matches(l);
	print_result(l, r);

	l = List_t{ 4, 3, 2, 1, 0 };
	print_list(l);
	r = get_max_number_of_matches(l);
	print_result(l, r);

	std::random_shuffle(l.begin(), l.end());
	print_list(l);
	r = get_max_number_of_matches(l);
	print_result(l, r);
}

int main() {
	auto init = gen_init(7);
	State_t final_state = generate_every_permutation_and_calc(init, &do_work);
	print_result(final_state.list, final_state.result);
}
