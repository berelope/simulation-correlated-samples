#include <iostream> // Needed for couting
#include <string> // Needed for couting strings

#include <random> // Needed for the random engine
#include <ctime> // Needed for the random engine

#include <cmath> // Needed for log, sin, etc.
#include <gsl/gsl_statistics_double.h> // GNU Scientific Library (mean, variance, etc.)

int main(int argc, const char * argv[]) {
    
    std::default_random_engine randomGenerator((unsigned int)time(NULL));
    
    const int samplesize = 100000;
    const double rho = 0.5;
    const double sigmax = 1;
    const double sigmay = sigmax; // Homoscedasticity
    const double mux = 10;
    const double muy = 5;
    
    double slope = rho * (sigmay/sigmax);
    double intercept = muy - slope * mux;
    double sigmae = sqrt(-((pow(rho, 2)-1)*sigmay)); // Standard error of estimation
    double meany;
    
    double dataX[samplesize];
    double dataY[samplesize];
    
    std::normal_distribution<double> rnorm(mux, sigmax); // Normality
    std::normal_distribution<double> rnorme(0, sigmae); // Random error with mean = 0 and sd = sigmae
    
    for (int i = 0; i < samplesize; i++){
        dataX[i] = rnorm(randomGenerator);
        dataY[i] = intercept + slope * dataX[i] + rnorme(randomGenerator);
    }
    
    std::cout << "Method 1:\n\tCorrelation: " << gsl_stats_correlation(dataX, 1, dataY, 1, samplesize) << std::endl;
    std::cout << "\tError: " << std::abs(rho-gsl_stats_correlation(dataX, 1, dataY, 1, samplesize)) << std::endl;
    std::cout << "\n";
    
    for (int i = 0; i < samplesize; i++){
        meany = dataX[i] * slope + intercept;
        std::normal_distribution<double> rnormy(meany, sigmae);
        dataY[i] = rnormy(randomGenerator);
    }
    
    std::cout << "Method 2:\n\tCorrelation: " << gsl_stats_correlation(dataX, 1, dataY, 1, samplesize) << std::endl;
    std::cout << "\tError: " << std::abs(rho-gsl_stats_correlation(dataX, 1, dataY, 1, samplesize)) << std::endl;
    std::cout << "\n";

    return 0;
    
}

