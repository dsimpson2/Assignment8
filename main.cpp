#include <iostream>
#include <stdexcept> 

class Field {
    // The base class owns the three Cartesian components.
protected:
    double* value;

public:
    Field() : value(new double[3]{0.0, 0.0, 0.0}) {} 

    Field(double x, double y, double z) : value(new double[3]{x, y, z}) {}

    Field(const Field& other) 
        : value(new double[3]{other.value[0], other.value[1], other.value[2]}) {}

    Field& operator=(const Field& other) {
        if (this != &other) {
            for (int i = 0; i < 3; ++i) {
                value[i] = other.value[i];
            }
        }
        return *this;
    }

    virtual ~Field() {
        delete[] value;
    }

    void printMagnitude() const {
        std::cout << "Components: (" << value[0] << ", " << value[1] << ", "
                  << value[2] << ")\n";
    }
};

class Electric_Field : public Field {
private:
    double calculatedField;

public:
    Electric_Field() : Field(), calculatedField(0.0) {}

    Electric_Field(double x, double y, double z)
        : Field(x, y, z), calculatedField(0.0) {}

    Electric_Field(const Electric_Field& other)
        : Field(other), calculatedField(other.calculatedField) {}

    Electric_Field& operator=(const Electric_Field& other) {
        if (this != &other) {
            Field::operator=(other);
            calculatedField = other.calculatedField;
        }
        return *this;
    }

    double calculateElectricField(double radius, double charge) {
        if (!(radius > 0.0)) {
            throw std::invalid_argument("Electric field radius must be positive.");
        }
        // Gauss' law for a spherical charge: E = Q / (4 pi r^2 epsilon_0).
        constexpr double epsilon0 = 8.8541878128e-12;
        constexpr double pi = 3.14159265358979323846;
        calculatedField = charge / (4.0 * pi * radius * radius * epsilon0);
        return calculatedField;
    }

    double getCalculatedField() const {
        return calculatedField;
    }

    Electric_Field operator+(const Electric_Field& other) const {
        Electric_Field result(value[0] + other.value[0],
                              value[1] + other.value[1],
                              value[2] + other.value[2]);
        result.calculatedField = calculatedField + other.calculatedField;
        return result;
    }

    friend std::ostream& operator<<(std::ostream& output,
                                    const Electric_Field& field) {
        return output << '(' << field.value[0] << ", " << field.value[1] << ", "
                      << field.value[2] << ')';
    }
};

class Magnetic_Field : public Field {
private:
    double calculatedField;

public:
    Magnetic_Field() : Field(), calculatedField(0.0) {}

    Magnetic_Field(double x, double y, double z)
        : Field(x, y, z), calculatedField(0.0) {}

    Magnetic_Field(const Magnetic_Field& other)
        : Field(other), calculatedField(other.calculatedField) {}

    Magnetic_Field& operator=(const Magnetic_Field& other) {
        if (this != &other) {
            Field::operator=(other);
            calculatedField = other.calculatedField;
        }
        return *this;
    }

    double calculateMagneticField(double radius, double current) {
        if (!(radius > 0.0)) {
            throw std::invalid_argument("Magnetic field radius must be positive.");
        }
        // Ampere's law for a long straight wire: B = I / (2 pi r mu_0).
        constexpr double mu0 = 1.25663706212e-6;
        constexpr double pi = 3.14159265358979323846;
        calculatedField = current / (2.0 * pi * radius * mu0);
        return calculatedField;
    }

    double getCalculatedField() const {
        return calculatedField;
    }

    Magnetic_Field operator+(const Magnetic_Field& other) const {
        Magnetic_Field result(value[0] + other.value[0],
                              value[1] + other.value[1],
                              value[2] + other.value[2]);
        result.calculatedField = calculatedField + other.calculatedField;
        return result;
    }

    friend std::ostream& operator<<(std::ostream& output,
                                    const Magnetic_Field& field) {
        return output << '(' << field.value[0] << ", " << field.value[1] << ", "
                      << field.value[2] << ')';
    }
};

int main() {
    // Initialize and print the three components of each field.
    Electric_Field electric(0.0, 1.0e5, 1.0e3);
    Magnetic_Field magnetic(2.0e-5, 0.0, 4.0e-5);

    std::cout << "Electric field ";
    electric.printMagnitude();
    std::cout << "Magnetic field ";
    magnetic.printMagnitude();

    // Calculate field strengths for a sample charge and current.
    const double electricMagnitude = electric.calculateElectricField(0.1, 1.0e-6);
    const double magneticMagnitude = magnetic.calculateMagneticField(0.05, 5.0);
    std::cout << "Calculated electric field: " << electricMagnitude << " N/C\n";
    std::cout << "Calculated magnetic field: " << magneticMagnitude << " T\n";

    // Demonstrate component-wise addition and stream output for both types.
    Electric_Field e2(1.0, 2.0, 3.0);
    Electric_Field e3(4.0, 5.0, 6.0);
    Electric_Field e1;
    e1 = e2 + e3;
    std::cout << "Electric field sum: " << e1 << '\n';

    Magnetic_Field b2(0.1, 0.2, 0.3);
    Magnetic_Field b3(0.4, 0.5, 0.6);
    Magnetic_Field b1;
    b1 = b2 + b3;
    std::cout << "Magnetic field sum: " << b1 << '\n';
}
