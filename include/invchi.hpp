#include <armadillo>
#include "utils.hpp"

struct RdfsParams
{
    double fmax;
    double sf;
    double minlambda;
    double dk;
    double ks;
    double alpha;
    int order;
    double v0;
    double dt;
    double dx;
    int ntr;
    int ns;
};

RdfsParams read_rdfs_params(const std::string& param_file);

arma::vec getcorrector(arma::mat& orig, arma::mat& filtered);

void rdfs_stretched(arma::vec& x, arma::cx_vec& X, double theta, int order, arma::vec& xbar);

void apply_rdfs(arma::mat& derdata, arma::mat& filtered, int ns, double alpha, int order);

arma::vec getcorrector(arma::mat& orig, arma::mat& filtered);