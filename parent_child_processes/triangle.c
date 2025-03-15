#include <stdio.h>

double countTriangleArea(double base, double height) {
    return 0.5 * base * height;
}

int main() {
    double base, height;
    
    printf("Insert: triangle base = ");
    scanf("%lf", &base);
    printf("height = ");
    scanf("%lf", &height);

    double area = countTriangleArea(base, height);
    printf("Triangle area = %f\n", area);
    
    return 0;
}
