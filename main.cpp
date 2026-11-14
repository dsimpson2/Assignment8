#include <iostream>
#include <stdexcept>


// Base Class: Field
// Stores the x, y, and z components of a three-dimensional field vector.

class Field {
protected:
    // Dynamic array containing the field components:
    // value[0] = x, value[1] = y, value[2] = z
    double* value;

public:
    // Default constructor initializes all components to zero.
    Field() : value(new double[3]{0.0, 0.0, 0.0}) {}

    // Parameterized constructor initializes the vector components.
    Field(double x, double y, double z)
        : value(new double[3]{x, y, z}) {}

    // Copy constructor performs a deep copy of the dynamic array.
    Field(const Field& other)
        : value(new double[3]{other.value[0],
                              other.value[1],
                              other.value[2]}) {}

    // Assignment operator copies values from another object.
    Field& operator=(const Field& other) {
        if (this != &other) {
            for (int i = 0; i < 3; ++i) {
                value[i] = other.value[i];
            }
        }
        return *this;
    }

    // Virtual destructor releases dynamically allocated memory.
    virtual ~Field() {
        delete[] value;
    }

    // Displays the vector components.
    void printMagnitude() const {
        std::cout << "Components: ("
                  << value[0] << ", "
                  << value[1] << ", "
                  << value[2] << ")\n";
    }
};

//=============================================================================
// Derived Class: Electric_Field
// Represents an electric field and provides electric field calculations.
//=============================================================================
class Electric_Field : public Field {
private:
    // Stores the most recently calculated electric field strength.
    double calculatedField;

public:
    // Default constructor.
    Electric_Field() : Field(), calculatedField(0.0) {}

    // Constructor initializing vector components.
    Electric_Field(double x, double y, double z)
        : Field(x, y, z), calculatedField(0.0) {}

    // Copy constructor.
    Electric_Field(const Electric_Field& other)
        : Field(other),
          calculatedField(other.calculatedField) {}

    // Assignment operator.
    Electric_Field& operator=(const Electric_Field& other) {
        if (this != &other) {
            Field::operator=(other);
            calculatedField = other.calculatedField;
        }
        return *this;
    }

    // Calculates electric field intensity using:
    // E = Q / (4πr²ε₀)
    double calculateElectricField(double radius, double charge) {

        // Radius must be positive.
        if (!(radius > 0.0)) {
            throw std::invalid_argument(
                "Electric field radius must be positive.");
        }

        constexpr double epsilon0 = 8.85e-12;
        constexpr double pi = 3.14;

        calculatedField =
            charge / (4.0 * pi * radius * radius * epsilon0);

        return calculatedField;
    }

    // Returns the latest calculated electric field.
    double getCalculatedField() const {
        return calculatedField;
    }

    // Overloads + operator for component-wise field addition.
    Electric_Field operator+(const Electric_Field& other) const {

        Electric_Field result(
            value[0] + other.value[0],
            value[1] + other.value[1],
            value[2] + other.value[2]);

        result.calculatedField =
            calculatedField + other.calculatedField;

        return result;
    }

    // Stream insertion operator for easy output.
    friend std::ostream& operator<<(std::ostream& output,
                                    const Electric_Field& field) {
        return output << '('
                      << field.value[0] << ", "
                      << field.value[1] << ", "
                      << field.value[2] << ')';
    }
};

//=============================================================================
// Derived Class: Magnetic_Field
// Represents a magnetic field and provides magnetic field calculations.
//=============================================================================
class Magnetic_Field : public Field {
private:
    // Stores the most recently calculated magnetic field strength.
    double calculatedField;

public:
    Magnetic_Field() : Field(), calculatedField(0.0) {}

    Magnetic_Field(double x, double y, double z)
        : Field(x, y, z), calculatedField(0.0) {}

    Magnetic_Field(const Magnetic_Field& other)
        : Field(other),
          calculatedField(other.calculatedField) {}

    Magnetic_Field& operator=(const Magnetic_Field& other) {
        if (this != &other) {
            Field::operator=(other);
            calculatedField = other.calculatedField;
        }
        return *this;
    }

    // Calculates magnetic field intensity using:
    // B = I / (2πrμ₀)
    double calculateMagneticField(double radius, double current) {

        // Radius must be positive.
        if (!(radius > 0.0)) {
            throw std::invalid_argument(
                "Magnetic field radius must be positive.");
        }

        constexpr double mu0 = 1.25e-6;
        constexpr double pi = 3.14;

        calculatedField =
            current / (2.0 * pi * radius * mu0);

        return calculatedField;
    }

    // Returns the latest calculated magnetic field.
    double getCalculatedField() const {
        return calculatedField;
    }

    // Overloads + operator for component-wise field addition.
    Magnetic_Field operator+(const Magnetic_Field& other) const {

        Magnetic_Field result(
            value[0] + other.value[0],
            value[1] + other.value[1],
            value[2] + other.value[2]);

        result.calculatedField =
            calculatedField + other.calculatedField;

        return result;
    }

    // Stream insertion operator for displaying field vectors.
    friend std::ostream& operator<<(std::ostream& output,
                                    const Magnetic_Field& field) {
        return output << '('
                      << field.value[0] << ", "
                      << field.value[1] << ", "
                      << field.value[2] << ')';
    }
};

int main() {

    // Create sample electric and magnetic field vectors.
    Electric_Field electric(0.0, 1.0e5, 1.0e3);
    Magnetic_Field magnetic(2.0e-5, 0.0, 4.0e-5);

    // Display vector components.
    std::cout << "Electric field ";
    electric.printMagnitude();

    std::cout << "Magnetic field ";
    magnetic.printMagnitude();

    // Calculate field magnitudes using physical formulas.
    const double electricMagnitude =
        electric.calculateElectricField(0.1, 1.0e-6);

    const double magneticMagnitude =
        magnetic.calculateMagneticField(0.05, 5.0);

    std::cout << "Calculated electric field: "
              << electricMagnitude << " N/C\n";

    std::cout << "Calculated magnetic field: "
              << magneticMagnitude << " T\n";

    // Demonstrate operator overloading for Electric_Field.
    Electric_Field e2(1.0, 2.0, 3.0);
    Electric_Field e3(4.0, 5.0, 6.0);

    Electric_Field e1;
    e1 = e2 + e3;

    std::cout << "Electric field sum: " << e1 << '\n';

    // Demonstrate operator overloading for Magnetic_Field.
    Magnetic_Field b2(0.1, 0.2, 0.3);
    Magnetic_Field b3(0.4, 0.5, 0.6);

    Magnetic_Field b1; 
    b1 = b2 + b3;

    std::cout << "Magnetic field sum: " << b1 << '\n'; 

    return 0;
}