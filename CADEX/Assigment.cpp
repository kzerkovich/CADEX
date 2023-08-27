#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <algorithm>
#include <memory>
#include <typeinfo>

using namespace std;

struct Point { //Структура, соответствующая точке кривой в трёхмерном пространстве.
	double x, y, z;
};

/*
	Структура, соответствующая вектору при вычислении первой производной кривой в трёхмерном пространстве.
	Компоненты структуры vectorDeriv полностью совпадают с компонентами структуры Point. Такое решение
	обусловлено различием их физического смысла.
*/
struct vectorDeriv { 
	double x, y, z;
};


ostream& operator<< (ostream& os, const Point& point) // Перегрузка оператора вывода для структуры Point.
{
	return os << point.x << " " << point.y << " " << point.z << " ";
}

ostream& operator<< (ostream& os, const vectorDeriv& vector) // Перегрузка оператора вывода для структуры vectorDeriv.
{
	return os << vector.x << " " << vector.y << " " << vector.z << " ";
}

class Curves { // Базовый класс всех кривых.
protected:
	/*
		Точка, относительно которой строится кривая. По умолчанию устанавливается в начале координат, 
		но может изменяться при передачи параметров в конструкторы классов.
	*/
	Point center = {0.0, 0.0, 0.0};

	Curves() {} // Конструкторы класса Curves.
	Curves(Point offset) { center = offset; }
public:
	virtual ~Curves() {} // Деструктор класса Curves.

	// Функция, возвращающая точку кривой при параметре t.
	virtual Point getPoint(double t) { return { 0.0, 0.0, 0.0 }; }
	// Функция, возвращающая первую производную при параметре t.
	virtual vectorDeriv getDerivative(double t) { return { 0.0, 0.0, 0.0 }; }
};

class Circle : public Curves { // Класс окружностей.
public:
	double radius = 0.0; // Радиус окружности.
	double rad() { return radius; } // Функция, возвращающая радиус окружности.

	Circle() {} // Конструкторы класса Circle.
	Circle(double r) : Curves() { radius = r; }
	Circle(Point offset, double r) : Curves(offset) { radius = r; }

	virtual ~Circle() {} // Деструктор класса Circle.

	/* 
		Функция, возвращающая точку окружности при параметре t.
		Параметрическое уравнение окружности:
			x(t) = x0 + r * cos(t),
			y(t) = y0 + r * sin(t),
			z(t) = z0, где (x0, y0, z0) - смещение относительно начала координат, r - радиус окружности.
	*/
	virtual Point getPoint(double t) {
		return {
			center.x + radius * cos(t),
			center.y + radius * sin(t),
			center.z
		};
	}
	/*
		Функция, возвращающая первую производную окружности при параметре t.
		Первая производная окружности:
			dx/dt = -r * sin(t),
			dy/dt = r * cos(t),
			dz/dt = 0.
	*/
	virtual vectorDeriv getDerivative(double t) {
		return {
			-1 * radius * sin(t),
			radius * cos(t),
			0.0
		};
	}
};

class Ellipse : public Curves { // Класс эллипсов.
private:
	// Радиусы эллипса.
	double a = 0.0;
	double b = 0.0;
public:
	Ellipse() {} // Конструкторы класса Ellipse.
	Ellipse(double first_radius, double second_radius) : Curves() { a = first_radius, b = second_radius; }
	Ellipse(Point offset, double first_radius, double second_radius) 
		: Curves(offset) { a = first_radius, b = second_radius;	}

	~Ellipse() {} // Деструктор класса Ellipse.

	/*
		Функция, возвращающая точку эллипса при параметре t.
		Параметрическое уравнение эллипса:
			x(t) = x0 + a * cos(t),
			y(t) = y0 + b * sin(t),
			z(t) = z0, где (x0, y0, z0) - смещение относительно начала координат, a, b - радиусы эллипса.
	*/
	virtual Point getPoint(double t) {
		return { 
			center.x + a * cos(t), 
			center.y + b * sin(t), 
			center.z 
		};
	}
	/*
		Функция, возвращающая первую производную эллипса при параметре t.
		Первая производная эллипса:
			dx/dt = -a * sin(t),
			dy/dt = b * cos(t),
			dz/dt = 0.
	*/
	virtual vectorDeriv getDerivative(double t) {
		return {
			-1 * a * sin(t),
			b * cos(t),
			0.0
		};
	}
};

class Helix : public Curves { // Класс спиралей.
private:
	double radius = 0.0; // Радиус спирали.
	double step = 0.0; // Шаг спирали.
public:
	Helix() {} // Конструкторы класса Helix.
	Helix(double r, double s) : Curves() { radius = r, step = s; }
	Helix(Point offset, double r, double s)
		: Curves(offset) { radius = r, step = s; }

	~Helix() {} // Деструктор класса Helix.

	/*
		Функция, возвращающая точку спирали при параметре t.
		Параметрическое уравнение спирали:
			x(t) = x0 + r * cos(t),
			y(t) = y0 + r * sin(t),
			z(t) = z0 + t * (step / 2 * pi), где (x0, y0, z0) - смещение относительно начала координат, r - радиус спирали, step - шаг спирали.
	*/
	virtual Point getPoint(double t) {
		return {
			center.x + radius * cos(t),
			center.y + radius * sin(t),
			center.z + t * step / (2 * M_PI)
		};
	}

	/*
		Функция, возвращающая первую производную эллипса при параметре t.
		Первая производная эллипса:
			dx/dt = -r * sin(t),
			dy/dt = r * cos(t),
			dz/dt = (step / 2 * pi).
	*/
	virtual vectorDeriv getDerivative(double t) {
		return {
			-1 * radius * sin(t),
			radius * cos(t),
			step / (2 * M_PI)
		};
	}
};

using PointerCurves = shared_ptr<Curves>; // Указатель на базовый класс.
using PointerCircle = shared_ptr<Circle>; // Указатель на класс Circle.

class Generator { // Класс для генерации псевдослучайных значений.
public:
	random_device rd;
	// Функция псевдослучайной генерации типа кривой.
	int typeOfCurves() { return rand() % 3; }

	// Равномерное распределение в диапазоне (-10, 10) для генерации псевдослучайных параметров кривых.
	uniform_real_distribution<double> generator{ -10 , 10 };

	PointerCurves genCurves() { // Генератор объектов всех классов.
		switch (typeOfCurves())
		{
		case 0:
			return make_shared<Circle>(Point{ generator(rd), generator(rd), generator(rd) }, abs(generator(rd)));
			break;
		case 1:
			return make_shared<Ellipse>(Point{ generator(rd), generator(rd), generator(rd) }, abs(generator(rd)), abs(generator(rd)));
			break;
		case 2:
			return make_shared<Helix>(Point{ generator(rd), generator(rd), generator(rd) }, abs(generator(rd)), abs(generator(rd)));
			break;
		default:
			return make_shared<Helix>(Point{ generator(rd), generator(rd), generator(rd) }, abs(generator(rd)), abs(generator(rd)));
		}
	}
};

// Компоратор для сортировки контейнера по длинам радиусов.
bool comp(PointerCircle a, PointerCircle b) {
	return a->rad() < b->rad();
}

int main() {

	srand(time(NULL));
	// Создание, псевдослучайная генерация данных и заполнение первого контейнера.
	vector<PointerCurves> first_container;
	Generator gen;
	for (size_t i = 0; i < 20; i++) {
		first_container.emplace_back(gen.genCurves());
	}
	
	vector<PointerCircle> second_container; // Второй контейнер только для объектов класса Circle.
	double sumOfRadii = 0.0; // Переменная для хранения суммы радиусов.

	for (auto &figure : first_container) {
		// Вывод точек и первых производных всех объектов в первом контейнере.
		const std::type_info& typeCurves{ typeid(*figure) };
		cout << "This is " << typeCurves.name() << endl
			<< "Coordinates of point at t = PI/4: "
			<< figure->getPoint(M_PI / 4) << endl
			<< "Derivative at t = PI/4: "
			<< figure->getDerivative(M_PI / 4) << "\n\n";
		/*
			Если объект является окружностью, то сохраняем во втором контейнере
			и сохраняем его радиус в переменной sumOfRadii.
		*/
		if (typeCurves.hash_code() == typeid(Circle).hash_code()) {
			second_container.emplace_back(dynamic_pointer_cast<Circle>(figure));
			sumOfRadii += second_container.back()->rad();
		}
	}
	//Сортировка второго контейнера.
	sort(second_container.begin(), second_container.end(), comp);
	cout << "Sorted vector of circles:" << endl;
	// Отсортированный второй контейнер.
	for (auto& figure : second_container)
		cout << figure->rad() << endl;
	// Сумма радиусов во втором контейнере.
	cout << "Sum of radii: " << sumOfRadii << endl;
}