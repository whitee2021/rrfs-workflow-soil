#include <iostream>
#include <netcdf>
#include <netcdf_mem.h>
#include <string>
#include <cstdlib>

using namespace std;
using namespace netCDF;
using namespace netCDF::exceptions;

int main() {
    char fname[138];
    const char* dir = getenv("PWD");
    sprintf(fname, "%s/mpasout.nc", dir);
    cout << fname << endl;

    int ncid;
    nc_open(fname,NC_WRITE,&ncid);

    int varid;
    auto values = new float[1][157859][9];
    auto slvalues = new float[1][157859];
    const size_t start[3] = {0,0,0};
    const size_t count[3] = {1,157859,9};

    for(int z=0; z<9; z++) {
        char vname[4];
        sprintf(vname, "SM%d",z+1);
        nc_inq_varid(ncid, vname, &varid);
        nc_get_var_float(ncid, varid, &slvalues[0][0]);
        for(int nc=0; nc<157859; nc++) {
            values[0][nc][z] = slvalues[0][nc];
        }
    }
    nc_inq_varid(ncid, "smois", &varid);
    nc_put_vara_float(ncid, varid, start, count, &values[0][0][0]);

    for(int z=0; z<9; z++) {
        char vname[4];
        sprintf(vname, "ST%d",z+1);
        nc_inq_varid(ncid, vname, &varid);
        nc_get_var_float(ncid, varid, &slvalues[0][0]);
        for(int nc=0; nc<157859; nc++) {
            values[0][nc][z] = slvalues[0][nc];
        }
    }
    nc_inq_varid(ncid, "tslb", &varid);
    nc_put_vara_float(ncid, varid, start, count, &values[0][0][0]);

    delete[] values;
    delete[] slvalues;

    nc_close(ncid);
}
