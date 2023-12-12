#include <vector>
#include <string>
#include <fstream>

#include <fmt/format.h>

void FillFile1(std::fstream& out, const std::vector<std::string>& names) {
	out << "-t ";
	for (const auto& n : names) {
		for (auto i = 2; i <= 5; ++i) {
			out << fmt::format("TestImages\\{}\\{}.jpg,", n, i);
		}
	}

	out << "| -g ";
	for (const auto& n : names) {
		out << fmt::format("TestImages\\{}\\1.jpg,", n);
	}
	out << "|";
}

void FillFile2(std::fstream& out, const std::vector<std::string>& names) {
	out << "-t ";
	for (const auto& n : names) {
		out << fmt::format("TestImages\\{}\\mean.jpg,", n);
	}

	out << "| -g ";
	for (const auto& n : names) {
		out << fmt::format("TestImages\\{}\\5.jpg,", n);
	}
	out << "|";
}

void FillFile3(std::fstream& out, const std::vector<std::string>& names) {
	std::string t = "-t ";
	std::string g = "-g ";

	auto iter = names.cbegin();
	t += fmt::format("TestImages\\{}\\image-{}.png", *iter, 0);
	g += fmt::format("TestImages\\{}\\image-{}.png", *iter, 9);

	for (auto i = 1; i < 400; ++i) {
		if (i % 10 == 9 && i != 9) {
			g += fmt::format(",TestImages\\{}\\image-{}.png", *iter, i);
			iter = std::next(iter);
		}
		else if (i % 10 != 9) {
			t += fmt::format(",TestImages\\{}\\image-{}.png", *iter, i);
		}
		else {
			iter = std::next(iter);
		}
	}

	out << t << " " << g;
}

void FillFile4(std::fstream& out, const std::vector<std::string>& names) {
	std::string t = "-t ";
	std::string g = "-g ";

	for (const auto& name : names) {
		for (int i = 1; i <= 10; ++i) {
			if (i <= 9) {
				t += t.size() == 3 ? fmt::format("Dataset\\{}-{}.png", name, i) : fmt::format(",Dataset\\{}-{}.png", name, i);
			}
			else {
				g += g.size() == 3 ? fmt::format("Dataset\\{}-{}.png", name, i) : fmt::format(",Dataset\\{}-{}.png", name, i);
			}
		}
	}

	out << t << " " << g;
}

int main() {
	std::fstream file("new_base.txt", std::fstream::out);

	//const std::vector<std::string> names = {
	//	"Carter", "Cane", "Clint",
	//	"Christofer", "Charles", "Caleb",
	//	"Carlos", "Catalina", "Connor",
	//	"Caroline", "Colin", "Cardell",
	//	"Cooper", "Christian", "Chase",
	//	"Calvin", "Colt", "Cezar",
	//	"Constantin", "Conroy", "Cleveland",
	//	"Callum", "Clyfford", "Cob",
	//	"Carman", "Chas", "Cooke",
	//	"Caroll", "Camdem", "Cuyler",
	//	"Cephas", "Clara", "Caden",
	//	"Cobham", "Charlotte", "Cael",
	//	"Cedrick", "Chip", "Cochran",
	//	"Claudius"
	//};

	const std::vector<std::string> names = {
		"Alan", "Alex", "Ali",
		"Amani", "Ben", "Carl",
		"Daniel", "Danny", "Erl",
		"Fabian", "Jin", "John",
		"Juan", "Karina", "Leo",
		"Li", "Luiza", "Mia",
		"Miranda", "Nickolas", "Olivia",
		"Pack", "Pamela", "Peter",
		"Phillip", "Pin", "Ralph",
		"Robert", "Robin", "Ron",
		"Saul", "Sean", "Steven",
		"Tanya", "Thomas", "Victor",
		"Vincent", "Xi"
	};

	FillFile4(file, names);

	return EXIT_SUCCESS;
}
