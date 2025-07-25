
#include "invchi.hpp"

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <data_file> <output_file> <param_file>\n";
        return EXIT_FAILURE;
    }

    const std::string dfile = argv[1];
    const std::string ofilename = argv[2];
    const std::string paramfile = argv[3];

    RdfsParams rdfs_p = read_rdfs_params(paramfile);

    arma::mat data;
    try {
        data.load(dfile, arma::raw_binary);
    } catch (const std::exception& e) {
        std::cerr << "Failed to load data from " << dfile << ": " << e.what() << '\n';
        return EXIT_FAILURE;
    }


    if (data.size() != rdfs_p.ntr * rdfs_p.ns) {
        std::cerr << "Error: data dimensions do not match parameters.\n";
        return EXIT_FAILURE;
    }

    data.reshape(rdfs_p.ns, rdfs_p.ntr);

    arma::mat filtered_data(arma::size(data), arma::fill::zeros);
    apply_rdfs(data, filtered_data, rdfs_p.ns, rdfs_p.alpha, rdfs_p.order);

    arma::vec chi = arma::abs(getcorrector(data, filtered_data));

    chi.save(ofilename, arma::raw_ascii);

    return EXIT_SUCCESS;
}

