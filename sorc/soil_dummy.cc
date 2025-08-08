#include <iostream>
#include <netcdf>
#include <netcdf_mem.h>
#include <string>
#include <cstdlib>

using namespace std;
using namespace netCDF;
using namespace netCDF::exceptions;

void make_dummy(char* fname) {
    cout << fname << endl;

    int ncid, nvars, ngatts, ndims, nunldims;
    nc_open(fname,NC_WRITE,&ncid);
    nc_inq(ncid, &ndims, &nvars, &ngatts, &nunldims);

    int ncells_id, time_id;
    nc_inq_dimid(ncid, "nCells", &ncells_id);
    nc_inq_dimid(ncid, "Time", &time_id);
    int dimids[2] = {time_id, ncells_id};

    int varid;
    auto values = new float[1][157859][9];
    auto slvalues = new float[1][157859];
    const size_t start[2] = {0,0};
    const size_t count[2] = {1,157859};
    int retval;

    nc_inq_varid(ncid, "smois", &varid);
    nc_get_var_float(ncid, varid, &values[0][0][0]);
    for(int z=0; z<9; z++) {
        for(int nc=0; nc<157859; nc++) {
            slvalues[0][nc] = values[0][nc][z];
        }
        char vname[4];
        sprintf(vname, "SM%d",z+1);
        nc_redef(ncid);
        retval = nc_def_var(ncid, vname, NC_FLOAT, 2, dimids, &varid);
        nc_enddef(ncid);
        if(retval == -42) {
            nc_inq_varid(ncid, vname, &varid);
        }
        nc_put_vara_float(ncid, varid, start, count, &slvalues[0][0]);
    }

    nc_inq_varid(ncid, "tslb", &varid);
    nc_get_var_float(ncid, varid, &values[0][0][0]);
    for(int z=0; z<9; z++) {
        for(int nc=0; nc<157859; nc++) {
            slvalues[0][nc] = values[0][nc][z];
        }
        char vname[4];
        sprintf(vname, "ST%d",z+1);
        nc_redef(ncid);
        retval = nc_def_var(ncid, vname, NC_FLOAT, 2, dimids, &varid);
        nc_enddef(ncid);
        if(retval == -42) {
            nc_inq_varid(ncid, vname, &varid);
        }
        nc_put_vara_float(ncid, varid, start, count, &slvalues[0][0]);
    }
    delete[] values;
    delete[] slvalues;

    nc_close(ncid);
}

int main(int argc, char *argv[]) {
    char fname[138];
    const char* timestr = argv[1]; //getenv("timestr");
    const char* comin = getenv("COMINrrfs");
    cout << timestr << endl;
    cout << comin << endl;
    const string timestr2(timestr);
    const string date = timestr2.substr(0,4) + timestr2.substr(5,2) + timestr2.substr(8,2);
    const int hour = (stoi(timestr2.substr(11,2)) - 1) % 24;

    for(int mem=1; mem<31; mem++) {
        sprintf(fname, "%s/rrfs.%s/%02d/fcst/enkf/mem%03d/mpasout.%s.nc",comin,date.c_str(),hour,mem,timestr);
        make_dummy(fname);
    }
    const char* prep_ic_umbrella = getenv("UMBRELLA_PREP_IC_DATA");
    sprintf(fname, "%s/mpasout.nc", prep_ic_umbrella);
    make_dummy(fname);
}

