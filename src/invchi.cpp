#include "invchi.hpp"


RdfsParams read_rdfs_params(const std::string& param_file){
    RdfsParams rdfs;
    rdfs.fmax = std::stod(readPar("fmax", param_file));
    rdfs.order = std::stoi(readPar("order", param_file));
    rdfs.dt = std::stod(readPar("dt", param_file));
    rdfs.dx = std::stod(readPar("dx", param_file));
    rdfs.ntr = std::stoi(readPar("ntr", param_file));
    rdfs.ns = std::stoi(readPar("ns", param_file));
    rdfs.sf = 1./rdfs.dt;

    rdfs.v0 = std::stod(readPar("v0", param_file));
    rdfs.minlambda = rdfs.v0 / rdfs.fmax;
    rdfs.dk = 1./rdfs.minlambda/double(rdfs.order);
    rdfs.ks = 1./rdfs.dx;
    rdfs.alpha = rdfs.ks / (rdfs.dk * (rdfs.ntr));

    //show all properties
    std::cerr << "----------------rdfs info-----------------\n";
    std::cerr << "fmax = " << rdfs.fmax << "\n";
    std::cerr << "sf = " << rdfs.sf << "\n";
    std::cerr << "minlambda = " << rdfs.minlambda << "\n";
    std::cerr << "dk = " << rdfs.dk << "\n";
    std::cerr << "ks = " << rdfs.ks << "\n";
    std::cerr << "alpha = " << rdfs.alpha << "\n";
    std::cerr << "order = " << rdfs.order << "\n";
    std::cerr << "dt = " << rdfs.dt << "\n";
    std::cerr << "dx = " << rdfs.dx << "\n";
    std::cerr << "ntr = " << rdfs.ntr << "\n";
    std::cerr << "ns = " << rdfs.ns << "\n";
    std::cerr << "v0 = " << rdfs.v0 << "\n";
    std::cerr << "------------------------------------------\n";
    return rdfs;
}


arma::vec getcorrector(arma::mat& orig, arma::mat& filtered){
    int nt=orig.n_rows;
	int nx=orig.n_cols;
	arma::vec gtb(nx, arma::fill::zeros), gtg(nx, arma::fill::zeros),chi(nx,arma::fill::zeros);
	
	for(int it=0; it<nt; it++){
		for(int ir=0; ir<nx; ir++){
			gtb(ir) += filtered(it, ir)*(orig(it,ir)-filtered(it,ir));
			gtg(ir) += filtered(it,ir)*filtered(it,ir);
		}
	}
	for(int ir=0; ir<nx; ir++) chi(ir) = gtb(ir)/gtg(ir);
	return chi;
}

void rdfs_stretched(arma::vec& x, arma::cx_vec& X, double theta, int order, arma::vec& xbar){
    int T = x.size();
    if(T==0){
        std::cerr << "data is empty\n";
        std::exit(1);
    }
    
    double  M = theta*T;
    int N = order*2 -1;

    arma::cx_mat W(T, N, arma::fill::zeros);

    for(int i=0; i<N; i++){
        for(int j=0; j<T; j++){
            double alpha;
            if(i<=N/2)
                alpha = 2. * arma::datum::pi * i * j / M;
            else
                alpha = 2. * arma::datum::pi * (i-N) * j / M;

            W(j, i) = std::complex<double>(cos(alpha), sin(alpha));
        }
    }

    arma::cx_mat Wt = arma::trans(W);
    arma::cx_vec Wtx = Wt*x;
    arma::cx_mat WtW = Wt*W;
    X = arma::solve(WtW, Wtx, arma::solve_opts::force_approx);
    
    xbar = arma::real(W*X);
}

void apply_rdfs(arma::mat& derdata, arma::mat& filtered, int ns, double alpha, int order){
    arma::cx_vec X;
    arma::vec xbar;
    for(int i=0; i<ns; i++)
    {
        arma::vec a = arma::conv_to<arma::vec>::from(derdata.row(i));
        rdfs_stretched(a, X, alpha, order, xbar);
        filtered.row(i) = arma::conv_to<arma::rowvec>::from(xbar);
    }
}

