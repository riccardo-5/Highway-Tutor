#include "../include/Highway.h"

Highway::Highway(const std::string& path) 
{
    std::ifstream file(path);
    if (!file.is_open()) 
    {
        std::cerr << "Errore: Impossibile trovare il file " << path << std::endl;
        exit(1);
    }

    double dist;
    char type;

    // Lettura diretta nei due vettori
    while (file >> dist >> type) {
        if (type == 'S') {
            junctions_.push_back({0, dist}); // ID provvisorio a 0
        } else if (type == 'V') {
            passages_.push_back({0, dist});  // ID provvisorio a 0
        }
    }
    file.close();

    // 1. Ordinamento per distanza chilometrica crescente 
    std::sort(junctions_.begin(), junctions_.end(), [](const Junction& a, const Junction& b) {
        return a.distFromOrigin_ < b.distFromOrigin_;
    });
    std::sort(passages_.begin(), passages_.end(), [](const Passage& a, const Passage& b) {
        return a.distFromOrigin_< b.distFromOrigin_;
    });

    // 2. Assegnazione degli ID univoci e indipendenti 
    for (size_t i = 0; i < junctions_.size(); ++i) {
        junctions_[i].id_ = static_cast<int>(i + 1);
    }
    for (size_t i = 0; i < passages_.size(); ++i) {
        passages_[i].id_ = static_cast<int>(i + 1);
    }

    // 3. Verifica dei vincoli strutturali
    validate();
}


std::vector<int> Highway::getPassagesInBetween(int startJunctionId, int endJunctionId) const 
{
    std::vector<int> result;

    // l'indice nel vettore e' shiftato di 1 perche inizia da 0
    double startKm = junctions_[startJunctionId - 1].distFromOrigin_;
    double endKm = junctions_[endJunctionId - 1].distFromOrigin_;

    // Iteriamo attraverso il vettore dei varchi (Passages).
    // Un varco è attraversato se la sua posizione è tra l'ingresso e l'uscita.
    for (const auto& p : passages_) {
        if (p.distFromOrigin_ > startKm && p.distFromOrigin_ < endKm) {
            result.push_back(p.id_);
        }
    }

    return result;
}


// PRIVATE HELPER

void Highway::validate() 
{
    // Devono essere presenti almeno due varchi
    if (passages_.size() < 2) {
        std::cerr << "Errore: Devono essere presenti almeno due varchi." << std::endl;
        exit(1);
    }

    // Svincolo obbligatorio prima del primo varco e dopo l'ultimo 
    if (junctions_.empty() || 
        junctions_.front().distFromOrigin_ >= passages_.front().distFromOrigin_ || 
        junctions_.back().distFromOrigin_ <= passages_.back().distFromOrigin_) {
        std::cerr << "Errore: Mancano gli svincoli necessari ai confini dei varchi." << std::endl;
        exit(1);
    }

    // Distanza minima di 1 km tra ogni svincolo e ogni varco 
    for (const auto& p : passages_) {
        for (const auto& j : junctions_) {
            if (std::abs(p.distFromOrigin_ - j.distFromOrigin_) < 1.0) {
                std::cerr << "Errore: Distanza varco-svincolo inferiore a 1 km al km " << p.distFromOrigin_ << std::endl;
                exit(1);
            }
        }
    }
}