#include "FixThermostat.h"

namespace COM::WFIXTURE {
FixThermostat::FixThermostat() {
    OperationPanel::Sudoku matrx = {
        {0, 0, 0},  //
        {0, 0, 0},  //
        {0, 0, 0}   //
    };
    Set_Sudoku(matrx);
}
}  // namespace COM::WFIXTURE
