// Safe parser for the Romana name/length/value parameter stream.
// Field mappings are generated from the supplied BinRead.cpp.
#include "toptions.h"
#include <nlohmann/json.hpp>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_set>
#include <type_traits>

namespace romana_bin_detail {
inline std::string bounded(const char *data, size_t capacity) {
    size_t n = 0;
    while (n < capacity && data[n]) ++n;
    return std::string(data, n);
}
template<size_t N> std::string bounded(const char (&data)[N]) { return bounded(data, N); }

// Copy at most the field's size. Leave missing array elements at defaults.
// A payload larger than the declared member is not silently applied: the
// structure layout may have changed between Romana versions.
template<typename T> void put(T &dst, const char *data, size_t len,
                              const std::string &field) {
    if (len > sizeof(dst))
        throw std::runtime_error("Romana parameter '" + field +
                                 "' exceeds destination field size");
    if (!std::is_array<T>::value && len != sizeof(dst))
        throw std::runtime_error("Romana parameter '" + field +
                                 "' has an invalid scalar size");
    if (std::is_array<T>::value && len % sizeof(typename std::remove_all_extents<T>::type) != 0)
        throw std::runtime_error("Romana parameter '" + field +
                                 "' has a truncated array element");
    if (len) std::memcpy(&dst, data, len);
}

template<typename T> void parse(T &obj, const char *data, size_t size,
                              bool (*apply)(T&,const std::string&,const char*,size_t),
                              std::unordered_set<std::string> *present = nullptr) {
    if (!data && size) throw std::runtime_error("Null parameter buffer");
    size_t pos = 0;
    while (pos < size) {
        // Romana may pad the end of the parameter block with zero bytes.
        if (size - pos < 2) {
            if (data[pos] == 0) return;
            throw std::runtime_error("Truncated Romana parameter name length");
        }
        uint16_t nameLen = 0;
        std::memcpy(&nameLen, data + pos, sizeof(nameLen)); pos += 2;
        if (!nameLen) {
            if (std::all_of(data + pos, data + size, [](char c){return c == 0;})) return;
            throw std::runtime_error("Zero Romana name length inside parameter block");
        }
        if (nameLen > size - pos)
            throw std::runtime_error("Romana parameter name exceeds header");
        const char *namePtr = data + pos;
        const size_t realLen = namePtr[nameLen-1] == 0 ? nameLen-1 : nameLen;
        std::string name(namePtr, realLen);
        pos += nameLen;
        if (size - pos < 2)
            throw std::runtime_error("Truncated Romana parameter data length");
        uint16_t dataLen = 0;
        std::memcpy(&dataLen, data + pos, 2); pos += 2;
        if (dataLen > size - pos)
            throw std::runtime_error("Romana parameter data exceeds header: " + name);
        if (apply(obj, name, data + pos, dataLen) && present)
            present->insert(name);
        pos += dataLen;
    }
}
} // namespace romana_bin_detail
inline bool apply_Coptions(Coptions &obj,const std::string &name,const char *data,size_t len) {
    if (name=="hS") { romana_bin_detail::put(obj.hS, data, len, name); return true; }
    else if (name=="Dt") { romana_bin_detail::put(obj.Dt, data, len, name); return true; }
    else if (name=="Pre") { romana_bin_detail::put(obj.Pre, data, len, name); return true; }
    else if (name=="Len") { romana_bin_detail::put(obj.Len, data, len, name); return true; }
    else if (name=="hD") { romana_bin_detail::put(obj.hD, data, len, name); return true; }
    else if (name=="Trg") { romana_bin_detail::put(obj.Trg, data, len, name); return true; }
    else if (name=="Drv") { romana_bin_detail::put(obj.Drv, data, len, name); return true; }
    else if (name=="Thr") { romana_bin_detail::put(obj.Thr, data, len, name); return true; }
    else if (name=="LT") { romana_bin_detail::put(obj.LT, data, len, name); return true; }
    else if (name=="G") { romana_bin_detail::put(obj.G, data, len, name); return true; }
    else if (name=="fdiv") { romana_bin_detail::put(obj.fdiv, data, len, name); return true; }
    else if (name=="AC") { romana_bin_detail::put(obj.AC, data, len, name); return true; }
    else if (name=="Inv") { romana_bin_detail::put(obj.Inv, data, len, name); return true; }
    else if (name=="on") { romana_bin_detail::put(obj.on, data, len, name); return true; }
    else if (name=="pls") { romana_bin_detail::put(obj.pls, data, len, name); return true; }
    else if (name=="coinc_w") { romana_bin_detail::put(obj.coinc_w, data, len, name); return true; }
    else if (name=="mult_w1") { romana_bin_detail::put(obj.mult_w1, data, len, name); return true; }
    else if (name=="mult_w2") { romana_bin_detail::put(obj.mult_w2, data, len, name); return true; }
    else if (name=="group") { romana_bin_detail::put(obj.group, data, len, name); return true; }
    else if (name=="RD") { romana_bin_detail::put(obj.RD, data, len, name); return true; }
    else if (name=="RMask") { romana_bin_detail::put(obj.RMask, data, len, name); return true; }
    else if (name=="forcewr") { romana_bin_detail::put(obj.forcewr, data, len, name); return true; }
    else if (name=="Trigger") { romana_bin_detail::put(obj.Trigger, data, len, name); return true; }
    else if (name=="Smpl") { romana_bin_detail::put(obj.Smpl, data, len, name); return true; }
    else if (name=="St_Per") { romana_bin_detail::put(obj.St_Per, data, len, name); return true; }
    else if (name=="F24") { romana_bin_detail::put(obj.F24, data, len, name); return true; }
    else if (name=="DTW") { romana_bin_detail::put(obj.DTW, data, len, name); return true; }
    else if (name=="F_start") { romana_bin_detail::put(obj.F_start, data, len, name); return true; }
    else if (name=="F_stop") { romana_bin_detail::put(obj.F_stop, data, len, name); return true; }
    else if (name=="device") { romana_bin_detail::put(obj.device, data, len, name); return true; }
    // Unknown fields and class/var markers are valid and safely skipped.
    return false;
}
inline void ReadFromBin(Coptions &obj, const char *data, size_t size,
                        std::unordered_set<std::string> *present = nullptr) {
    romana_bin_detail::parse(obj, data, size, apply_Coptions, present);
}
inline bool apply_Toptions(Toptions &obj,const std::string &name,const char *data,size_t len) {
    if (name=="gitver") { romana_bin_detail::put(obj.gitver, data, len, name); return true; }
    else if (name=="ver_po") { romana_bin_detail::put(obj.ver_po, data, len, name); return true; }
    else if (name=="maxch") { romana_bin_detail::put(obj.maxch, data, len, name); return true; }
    else if (name=="maxtp") { romana_bin_detail::put(obj.maxtp, data, len, name); return true; }
    else if (name=="chkall") { romana_bin_detail::put(obj.chkall, data, len, name); return true; }
    else if (name=="star") { romana_bin_detail::put(obj.star, data, len, name); return true; }
    else if (name=="chtype") { romana_bin_detail::put(obj.chtype, data, len, name); return true; }
    else if (name=="dsp") { romana_bin_detail::put(obj.dsp, data, len, name); return true; }
    else if (name=="Dsp") { romana_bin_detail::put(obj.Dsp, data, len, name); return true; }
    else if (name=="Pls") { romana_bin_detail::put(obj.Pls, data, len, name); return true; }
    else if (name=="St") { romana_bin_detail::put(obj.St, data, len, name); return true; }
    else if (name=="Ms") { romana_bin_detail::put(obj.Ms, data, len, name); return true; }
    else if (name=="Grp") { romana_bin_detail::put(obj.Grp, data, len, name); return true; }
    else if (name=="sS") { romana_bin_detail::put(obj.sS, data, len, name); return true; }
    else if (name=="sDrv") { romana_bin_detail::put(obj.sDrv, data, len, name); return true; }
    else if (name=="sThr") { romana_bin_detail::put(obj.sThr, data, len, name); return true; }
    else if (name=="sLT") { romana_bin_detail::put(obj.sLT, data, len, name); return true; }
    else if (name=="sD") { romana_bin_detail::put(obj.sD, data, len, name); return true; }
    else if (name=="B1") { romana_bin_detail::put(obj.B1, data, len, name); return true; }
    else if (name=="B2") { romana_bin_detail::put(obj.B2, data, len, name); return true; }
    else if (name=="P1") { romana_bin_detail::put(obj.P1, data, len, name); return true; }
    else if (name=="P2") { romana_bin_detail::put(obj.P2, data, len, name); return true; }
    else if (name=="dTm") { romana_bin_detail::put(obj.dTm, data, len, name); return true; }
    else if (name=="Pile") { romana_bin_detail::put(obj.Pile, data, len, name); return true; }
    else if (name=="sTg") { romana_bin_detail::put(obj.sTg, data, len, name); return true; }
    else if (name=="DD") { romana_bin_detail::put(obj.DD, data, len, name); return true; }
    else if (name=="FF") { romana_bin_detail::put(obj.FF, data, len, name); return true; }
    else if (name=="T1") { romana_bin_detail::put(obj.T1, data, len, name); return true; }
    else if (name=="T2") { romana_bin_detail::put(obj.T2, data, len, name); return true; }
    else if (name=="W1") { romana_bin_detail::put(obj.W1, data, len, name); return true; }
    else if (name=="W2") { romana_bin_detail::put(obj.W2, data, len, name); return true; }
    else if (name=="Mt") { romana_bin_detail::put(obj.Mt, data, len, name); return true; }
    else if (name=="Mr") { romana_bin_detail::put(obj.Mr, data, len, name); return true; }
    else if (name=="Fpath") { romana_bin_detail::put(obj.Fpath, data, len, name); return true; }
    else if (name=="calibr_t") { romana_bin_detail::put(obj.calibr_t, data, len, name); return true; }
    else if (name=="E0") { romana_bin_detail::put(obj.E0, data, len, name); return true; }
    else if (name=="E1") { romana_bin_detail::put(obj.E1, data, len, name); return true; }
    else if (name=="E2") { romana_bin_detail::put(obj.E2, data, len, name); return true; }
    else if (name=="Pz") { romana_bin_detail::put(obj.Pz, data, len, name); return true; }
    else if (name=="E_auto") { romana_bin_detail::put(obj.E_auto, data, len, name); return true; }
    else if (name=="adj") { romana_bin_detail::put(obj.adj, data, len, name); return true; }
    else if (name=="elim1") { romana_bin_detail::put(obj.elim1, data, len, name); return true; }
    else if (name=="elim2") { romana_bin_detail::put(obj.elim2, data, len, name); return true; }
    else if (name=="T_acq") { romana_bin_detail::put(obj.T_acq, data, len, name); return true; }
    else if (name=="Period") { romana_bin_detail::put(obj.Period, data, len, name); return true; }
    else if (name=="nthreads") { romana_bin_detail::put(obj.nthreads, data, len, name); return true; }
    else if (name=="Nchan") { romana_bin_detail::put(obj.Nchan, data, len, name); return true; }
    else if (name=="Tstart") { romana_bin_detail::put(obj.Tstart, data, len, name); return true; }
    else if (name=="Tstop") { romana_bin_detail::put(obj.Tstop, data, len, name); return true; }
    else if (name=="tsleep") { romana_bin_detail::put(obj.tsleep, data, len, name); return true; }
    else if (name=="usb_size") { romana_bin_detail::put(obj.usb_size, data, len, name); return true; }
    else if (name=="rbuf_size") { romana_bin_detail::put(obj.rbuf_size, data, len, name); return true; }
    else if (name=="ibuf_size") { romana_bin_detail::put(obj.ibuf_size, data, len, name); return true; }
    else if (name=="decbuf_size") { romana_bin_detail::put(obj.decbuf_size, data, len, name); return true; }
    else if (name=="rawbuf_size") { romana_bin_detail::put(obj.rawbuf_size, data, len, name); return true; }
    else if (name=="b_stack") { romana_bin_detail::put(obj.b_stack, data, len, name); return true; }
    else if (name=="b_norm") { romana_bin_detail::put(obj.b_norm, data, len, name); return true; }
    else if (name=="xdiv") { romana_bin_detail::put(obj.xdiv, data, len, name); return true; }
    else if (name=="ydiv") { romana_bin_detail::put(obj.ydiv, data, len, name); return true; }
    else if (name=="icheck") { romana_bin_detail::put(obj.icheck, data, len, name); return true; }
    else if (name=="directraw") { romana_bin_detail::put(obj.directraw, data, len, name); return true; }
    else if (name=="checkdsp") { romana_bin_detail::put(obj.checkdsp, data, len, name); return true; }
    else if (name=="raw_write") { romana_bin_detail::put(obj.raw_write, data, len, name); return true; }
    else if (name=="fProc") { romana_bin_detail::put(obj.fProc, data, len, name); return true; }
    else if (name=="fTxt") { romana_bin_detail::put(obj.fTxt, data, len, name); return true; }
    else if (name=="fBin") { romana_bin_detail::put(obj.fBin, data, len, name); return true; }
    else if (name=="dec_write") { romana_bin_detail::put(obj.dec_write, data, len, name); return true; }
    else if (name=="root_write") { romana_bin_detail::put(obj.root_write, data, len, name); return true; }
    else if (name=="raw_compr") { romana_bin_detail::put(obj.raw_compr, data, len, name); return true; }
    else if (name=="dec_compr") { romana_bin_detail::put(obj.dec_compr, data, len, name); return true; }
    else if (name=="root_compr") { romana_bin_detail::put(obj.root_compr, data, len, name); return true; }
    else if (name=="dec_format") { romana_bin_detail::put(obj.dec_format, data, len, name); return true; }
    else if (name=="dec_mask") { romana_bin_detail::put(obj.dec_mask, data, len, name); return true; }
    else if (name=="Filename") { romana_bin_detail::put(obj.Filename, data, len, name); return true; }
    else if (name=="Daqlog") { romana_bin_detail::put(obj.Daqlog, data, len, name); return true; }
    else if (name=="Analog") { romana_bin_detail::put(obj.Analog, data, len, name); return true; }
    else if (name=="Comment") { romana_bin_detail::put(obj.Comment, data, len, name); return true; }
    else if (name=="addrandom") { romana_bin_detail::put(obj.addrandom, data, len, name); return true; }
    else if (name=="hideself") { romana_bin_detail::put(obj.hideself, data, len, name); return true; }
    else if (name=="wdog_timer") { romana_bin_detail::put(obj.wdog_timer, data, len, name); return true; }
    else if (name=="wdog1") { romana_bin_detail::put(obj.wdog1, data, len, name); return true; }
    else if (name=="wdog2") { romana_bin_detail::put(obj.wdog2, data, len, name); return true; }
    else if (name=="ch_name") { romana_bin_detail::put(obj.ch_name, data, len, name); return true; }
    else if (name=="drawopt") { romana_bin_detail::put(obj.drawopt, data, len, name); return true; }
    else if (name=="ev_min") { romana_bin_detail::put(obj.ev_min, data, len, name); return true; }
    else if (name=="ev_max") { romana_bin_detail::put(obj.ev_max, data, len, name); return true; }
    else if (name=="buf_lag") { romana_bin_detail::put(obj.buf_lag, data, len, name); return true; }
    else if (name=="tgate") { romana_bin_detail::put(obj.tgate, data, len, name); return true; }
    else if (name=="tveto") { romana_bin_detail::put(obj.tveto, data, len, name); return true; }
    else if (name=="mult1") { romana_bin_detail::put(obj.mult1, data, len, name); return true; }
    else if (name=="mult2") { romana_bin_detail::put(obj.mult2, data, len, name); return true; }
    else if (name=="maintrig") { romana_bin_detail::put(obj.maintrig, data, len, name); return true; }
    else if (name=="seltab") { romana_bin_detail::put(obj.seltab, data, len, name); return true; }
    else if (name=="start_ch") { romana_bin_detail::put(obj.start_ch, data, len, name); return true; }
    else if (name=="ntof_period") { romana_bin_detail::put(obj.ntof_period, data, len, name); return true; }
    else if (name=="TofZero") { romana_bin_detail::put(obj.TofZero, data, len, name); return true; }
    else if (name=="Prof_type") { romana_bin_detail::put(obj.Prof_type, data, len, name); return true; }
    else if (name=="Ing_type") { romana_bin_detail::put(obj.Ing_type, data, len, name); return true; }
    else if (name=="prof_nx") { romana_bin_detail::put(obj.prof_nx, data, len, name); return true; }
    else if (name=="prof_ny") { romana_bin_detail::put(obj.prof_ny, data, len, name); return true; }
    else if (name=="Ing_x") { romana_bin_detail::put(obj.Ing_x, data, len, name); return true; }
    else if (name=="Ing_y") { romana_bin_detail::put(obj.Ing_y, data, len, name); return true; }
    else if (name=="Prof_x") { romana_bin_detail::put(obj.Prof_x, data, len, name); return true; }
    else if (name=="Prof_y") { romana_bin_detail::put(obj.Prof_y, data, len, name); return true; }
    else if (name=="Prof64") { romana_bin_detail::put(obj.Prof64, data, len, name); return true; }
    else if (name=="Prof64_TSP") { romana_bin_detail::put(obj.Prof64_TSP, data, len, name); return true; }
    else if (name=="Prof64_W") { romana_bin_detail::put(obj.Prof64_W, data, len, name); return true; }
    else if (name=="Prof64_THR") { romana_bin_detail::put(obj.Prof64_THR, data, len, name); return true; }
    else if (name=="Prof64_GAT") { romana_bin_detail::put(obj.Prof64_GAT, data, len, name); return true; }
    else if (name=="Prof64_X") { romana_bin_detail::put(obj.Prof64_X, data, len, name); return true; }
    else if (name=="Prof64_Y") { romana_bin_detail::put(obj.Prof64_Y, data, len, name); return true; }
    else if (name=="num_events") { romana_bin_detail::put(obj.num_events, data, len, name); return true; }
    else if (name=="num_buf") { romana_bin_detail::put(obj.num_buf, data, len, name); return true; }
    else if (name=="b_logy") { romana_bin_detail::put(obj.b_logy, data, len, name); return true; }
    else if (name=="b_stat") { romana_bin_detail::put(obj.b_stat, data, len, name); return true; }
    else if (name=="b_gcuts") { romana_bin_detail::put(obj.b_gcuts, data, len, name); return true; }
    else if (name=="b_roi") { romana_bin_detail::put(obj.b_roi, data, len, name); return true; }
    else if (name=="b_fpeaks") { romana_bin_detail::put(obj.b_fpeaks, data, len, name); return true; }
    else if (name=="b_deriv") { romana_bin_detail::put(obj.b_deriv, data, len, name); return true; }
    else if (name=="b_peak") { romana_bin_detail::put(obj.b_peak, data, len, name); return true; }
    else if (name=="adcm_period") { romana_bin_detail::put(obj.adcm_period, data, len, name); return true; }
    else if (name=="ncuts") { romana_bin_detail::put(obj.ncuts, data, len, name); return true; }
    else if (name=="pcuts") { romana_bin_detail::put(obj.pcuts, data, len, name); return true; }
    else if (name=="gcut") { romana_bin_detail::put(obj.gcut, data, len, name); return true; }
    else if (name=="cut_form") { romana_bin_detail::put(obj.cut_form, data, len, name); return true; }
    else if (name=="formula") { romana_bin_detail::put(obj.formula, data, len, name); return true; }
    else if (name=="SimSim") { romana_bin_detail::put(obj.SimSim, data, len, name); return true; }
    else if (name=="Peak_thr") { romana_bin_detail::put(obj.Peak_thr, data, len, name); return true; }
    else if (name=="Peak_wid") { romana_bin_detail::put(obj.Peak_wid, data, len, name); return true; }
    else if (name=="Peak_bwidth") { romana_bin_detail::put(obj.Peak_bwidth, data, len, name); return true; }
    else if (name=="Peak_maxpeaks") { romana_bin_detail::put(obj.Peak_maxpeaks, data, len, name); return true; }
    else if (name=="Peak_smooth") { romana_bin_detail::put(obj.Peak_smooth, data, len, name); return true; }
    else if (name=="Peak_show_sm") { romana_bin_detail::put(obj.Peak_show_sm, data, len, name); return true; }
    else if (name=="Peak_use_mean") { romana_bin_detail::put(obj.Peak_use_mean, data, len, name); return true; }
    else if (name=="Peak_print") { romana_bin_detail::put(obj.Peak_print, data, len, name); return true; }
    else if (name=="OptPar") { romana_bin_detail::put(obj.OptPar, data, len, name); return true; }
    else if (name=="wrk_check") { romana_bin_detail::put(obj.wrk_check, data, len, name); return true; }
    else if (name=="Nrows") { romana_bin_detail::put(obj.Nrows, data, len, name); return true; }
    else if (name=="ScrollPos") { romana_bin_detail::put(obj.ScrollPos, data, len, name); return true; }
    else if (name=="hx_slider") { romana_bin_detail::put(obj.hx_slider, data, len, name); return true; }
    else if (name=="hy_slider") { romana_bin_detail::put(obj.hy_slider, data, len, name); return true; }
    // Unknown fields and class/var markers are valid and safely skipped.
    return false;
}
inline void ReadFromBin(Toptions &obj, const char *data, size_t size,
                        std::unordered_set<std::string> *present = nullptr) {
    romana_bin_detail::parse(obj, data, size, apply_Toptions, present);
}

void ToJSON(Coptions &obj, nlohmann::json &jsn)
{
	jsn["Class"]="Coptions";
	std::vector<unsigned int> device_values;
	for (auto v : obj.device) device_values.push_back(v);
	jsn["device"] = device_values;
	vector<int> hS_vec;
	hS_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		hS_vec[i0]=obj.hS[i0];
	}
	jsn["hS"]=hS_vec;
	vector<int> Dt_vec;
	Dt_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		Dt_vec[i0]=obj.Dt[i0];
	}
	jsn["Dt"]=Dt_vec;
	vector<int> Pre_vec;
	Pre_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		Pre_vec[i0]=obj.Pre[i0];
	}
	jsn["Pre"]=Pre_vec;
	vector<int> Len_vec;
	Len_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		Len_vec[i0]=obj.Len[i0];
	}
	jsn["Len"]=Len_vec;
	vector<int> hD_vec;
	hD_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		hD_vec[i0]=obj.hD[i0];
	}
	jsn["hD"]=hD_vec;
	vector<int> Trg_vec;
	Trg_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		Trg_vec[i0]=obj.Trg[i0];
	}
	jsn["Trg"]=Trg_vec;
	vector<int> Drv_vec;
	Drv_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		Drv_vec[i0]=obj.Drv[i0];
	}
	jsn["Drv"]=Drv_vec;
	vector<int> Thr_vec;
	Thr_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		Thr_vec[i0]=obj.Thr[i0];
	}
	jsn["Thr"]=Thr_vec;
	vector<int> LT_vec;
	LT_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		LT_vec[i0]=obj.LT[i0];
	}
	jsn["LT"]=LT_vec;
	vector<int> G_vec;
	G_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		G_vec[i0]=obj.G[i0];
	}
	jsn["G"]=G_vec;
	vector<int> fdiv_vec;
	fdiv_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		fdiv_vec[i0]=obj.fdiv[i0];
	}
	jsn["fdiv"]=fdiv_vec;
	vector<bool> AC_vec;
	AC_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		AC_vec[i0]=obj.AC[i0];
	}
	jsn["AC"]=AC_vec;
	vector<bool> Inv_vec;
	Inv_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		Inv_vec[i0]=obj.Inv[i0];
	}
	jsn["Inv"]=Inv_vec;
	vector<bool> on_vec;
	on_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		on_vec[i0]=obj.on[i0];
	}
	jsn["on"]=on_vec;
	vector<bool> pls_vec;
	pls_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		pls_vec[i0]=obj.pls[i0];
	}
	jsn["pls"]=pls_vec;
	vector<int> coinc_w_vec;
	coinc_w_vec.resize(2);
	for(int i0=0;i0<2;i0++)
	{
		coinc_w_vec[i0]=obj.coinc_w[i0];
	}
	jsn["coinc_w"]=coinc_w_vec;
	vector<int> mult_w1_vec;
	mult_w1_vec.resize(2);
	for(int i0=0;i0<2;i0++)
	{
		mult_w1_vec[i0]=obj.mult_w1[i0];
	}
	jsn["mult_w1"]=mult_w1_vec;
	vector<int> mult_w2_vec;
	mult_w2_vec.resize(2);
	for(int i0=0;i0<2;i0++)
	{
		mult_w2_vec[i0]=obj.mult_w2[i0];
	}
	jsn["mult_w2"]=mult_w2_vec;
	vector<vector<bool> > group_vec;
	group_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		group_vec[i0].resize(2);
		for(int i1=0;i1<2;i1++)
		{
			group_vec[i0][i1]=obj.group[i0][i1];
		}
	}
	jsn["group"]=group_vec;
	vector<int> RD_vec;
	RD_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		RD_vec[i0]=obj.RD[i0];
	}
	jsn["RD"]=RD_vec;
	jsn["RMask"]=obj.RMask;
	jsn["forcewr"]=obj.forcewr;
	jsn["Trigger"]=obj.Trigger;
	jsn["Smpl"]=obj.Smpl;
	jsn["St_Per"]=obj.St_Per;
	jsn["F24"]=obj.F24;
	vector<int> DTW_vec;
	DTW_vec.resize(2);
	for(int i0=0;i0<2;i0++)
	{
		DTW_vec[i0]=obj.DTW[i0];
	}
	jsn["DTW"]=DTW_vec;
	jsn["F_start"]=obj.F_start;
	jsn["F_stop"]=obj.F_stop;
}
void ToJSON(Toptions &obj, nlohmann::json &jsn)
{
	jsn["Class"]="Toptions";
	jsn["ver_po"] = static_cast<int>(obj.ver_po);
	jsn["gitver"]=romana_bin_detail::bounded(obj.gitver);
	jsn["maxch"]=obj.maxch;
	jsn["maxtp"]=obj.maxtp;
	jsn["chkall"]=obj.chkall;
	vector<bool> star_vec;
	star_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		star_vec[i0]=obj.star[i0];
	}
	jsn["star"]=star_vec;
	vector<int> chtype_vec;
	chtype_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		chtype_vec[i0]=obj.chtype[i0];
	}
	jsn["chtype"]=chtype_vec;
	vector<bool> dsp_vec;
	dsp_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		dsp_vec[i0]=obj.dsp[i0];
	}
	jsn["dsp"]=dsp_vec;
	vector<int> Dsp_vec;
	Dsp_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		Dsp_vec[i0]=obj.Dsp[i0];
	}
	jsn["Dsp"]=Dsp_vec;
	vector<bool> Pls_vec;
	Pls_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		Pls_vec[i0]=obj.Pls[i0];
	}
	jsn["Pls"]=Pls_vec;
	vector<bool> St_vec;
	St_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		St_vec[i0]=obj.St[i0];
	}
	jsn["St"]=St_vec;
	vector<bool> Ms_vec;
	Ms_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		Ms_vec[i0]=obj.Ms[i0];
	}
	jsn["Ms"]=Ms_vec;
	vector<vector<bool> > Grp_vec;
	Grp_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		Grp_vec[i0].resize(4);
		for(int i1=0;i1<4;i1++)
		{
			Grp_vec[i0][i1]=obj.Grp[i0][i1];
		}
	}
	jsn["Grp"]=Grp_vec;
	vector<int> sS_vec;
	sS_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		sS_vec[i0]=obj.sS[i0];
	}
	jsn["sS"]=sS_vec;
	vector<int> sDrv_vec;
	sDrv_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		sDrv_vec[i0]=obj.sDrv[i0];
	}
	jsn["sDrv"]=sDrv_vec;
	vector<int> sThr_vec;
	sThr_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		sThr_vec[i0]=obj.sThr[i0];
	}
	jsn["sThr"]=sThr_vec;
	vector<int> sLT_vec;
	sLT_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		sLT_vec[i0]=obj.sLT[i0];
	}
	jsn["sLT"]=sLT_vec;
	vector<float> sD_vec;
	sD_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		sD_vec[i0]=obj.sD[i0];
	}
	jsn["sD"]=sD_vec;
	vector<int> B1_vec;
	B1_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		B1_vec[i0]=obj.B1[i0];
	}
	jsn["B1"]=B1_vec;
	vector<int> B2_vec;
	B2_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		B2_vec[i0]=obj.B2[i0];
	}
	jsn["B2"]=B2_vec;
	vector<int> P1_vec;
	P1_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		P1_vec[i0]=obj.P1[i0];
	}
	jsn["P1"]=P1_vec;
	vector<int> P2_vec;
	P2_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		P2_vec[i0]=obj.P2[i0];
	}
	jsn["P2"]=P2_vec;
	vector<int> dTm_vec;
	dTm_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		dTm_vec[i0]=obj.dTm[i0];
	}
	jsn["dTm"]=dTm_vec;
	vector<int> Pile_vec;
	Pile_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		Pile_vec[i0]=obj.Pile[i0];
	}
	jsn["Pile"]=Pile_vec;
	vector<int> sTg_vec;
	sTg_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		sTg_vec[i0]=obj.sTg[i0];
	}
	jsn["sTg"]=sTg_vec;
	vector<int> DD_vec;
	DD_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		DD_vec[i0]=obj.DD[i0];
	}
	jsn["DD"]=DD_vec;
	vector<int> FF_vec;
	FF_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		FF_vec[i0]=obj.FF[i0];
	}
	jsn["FF"]=FF_vec;
	vector<int> T1_vec;
	T1_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		T1_vec[i0]=obj.T1[i0];
	}
	jsn["T1"]=T1_vec;
	vector<int> T2_vec;
	T2_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		T2_vec[i0]=obj.T2[i0];
	}
	jsn["T2"]=T2_vec;
	vector<int> W1_vec;
	W1_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		W1_vec[i0]=obj.W1[i0];
	}
	jsn["W1"]=W1_vec;
	vector<int> W2_vec;
	W2_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		W2_vec[i0]=obj.W2[i0];
	}
	jsn["W2"]=W2_vec;
	vector<int> Mt_vec;
	Mt_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		Mt_vec[i0]=obj.Mt[i0];
	}
	jsn["Mt"]=Mt_vec;
	vector<int> Mr_vec;
	Mr_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		Mr_vec[i0]=obj.Mr[i0];
	}
	jsn["Mr"]=Mr_vec;
	vector<float> Fpath_vec;
	Fpath_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		Fpath_vec[i0]=obj.Fpath[i0];
	}
	jsn["Fpath"]=Fpath_vec;
	vector<int> calibr_t_vec;
	calibr_t_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		calibr_t_vec[i0]=obj.calibr_t[i0];
	}
	jsn["calibr_t"]=calibr_t_vec;
	vector<float> E0_vec;
	E0_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		E0_vec[i0]=obj.E0[i0];
	}
	jsn["E0"]=E0_vec;
	vector<float> E1_vec;
	E1_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		E1_vec[i0]=obj.E1[i0];
	}
	jsn["E1"]=E1_vec;
	vector<float> E2_vec;
	E2_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		E2_vec[i0]=obj.E2[i0];
	}
	jsn["E2"]=E2_vec;
	vector<int> Pz_vec;
	Pz_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		Pz_vec[i0]=obj.Pz[i0];
	}
	jsn["Pz"]=Pz_vec;
	jsn["E_auto"]=obj.E_auto;
	vector<vector<float> > adj_vec;
	adj_vec.resize(261);
	for(int i0=0;i0<261;i0++)
	{
		adj_vec[i0].resize(3);
		for(int i1=0;i1<3;i1++)
		{
			adj_vec[i0][i1]=obj.adj[i0][i1];
		}
	}
	jsn["adj"]=adj_vec;
	vector<float> elim1_vec;
	elim1_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		elim1_vec[i0]=obj.elim1[i0];
	}
	jsn["elim1"]=elim1_vec;
	vector<float> elim2_vec;
	elim2_vec.resize(265);
	for(int i0=0;i0<265;i0++)
	{
		elim2_vec[i0]=obj.elim2[i0];
	}
	jsn["elim2"]=elim2_vec;
	jsn["T_acq"]=obj.T_acq;
	jsn["Period"]=obj.Period;
	jsn["nthreads"]=obj.nthreads;
	jsn["Nchan"]=obj.Nchan;
	jsn["Tstart"]=obj.Tstart;
	jsn["Tstop"]=obj.Tstop;
	jsn["tsleep"]=obj.tsleep;
	jsn["usb_size"]=obj.usb_size;
	jsn["rbuf_size"]=obj.rbuf_size;
	jsn["ibuf_size"]=obj.ibuf_size;
	jsn["decbuf_size"]=obj.decbuf_size;
	jsn["rawbuf_size"]=obj.rawbuf_size;
	jsn["b_stack"]=obj.b_stack;
	jsn["b_norm"]=obj.b_norm;
	jsn["xdiv"]=obj.xdiv;
	jsn["ydiv"]=obj.ydiv;
	jsn["icheck"]=obj.icheck;
	jsn["directraw"]=obj.directraw;
	jsn["checkdsp"]=obj.checkdsp;
	jsn["raw_write"]=obj.raw_write;
	jsn["fProc"]=obj.fProc;
	jsn["fTxt"]=obj.fTxt;
	jsn["fBin"]=obj.fBin;
	jsn["dec_write"]=obj.dec_write;
	jsn["root_write"]=obj.root_write;
	jsn["raw_compr"]=obj.raw_compr;
	jsn["dec_compr"]=obj.dec_compr;
	jsn["root_compr"]=obj.root_compr;
	jsn["dec_format"]=obj.dec_format;
	jsn["dec_mask"]=romana_bin_detail::bounded(obj.dec_mask);
	jsn["Filename"]=romana_bin_detail::bounded(obj.Filename);
	jsn["Daqlog"]=romana_bin_detail::bounded(obj.Daqlog);
	jsn["Analog"]=romana_bin_detail::bounded(obj.Analog);
	jsn["Comment"]=romana_bin_detail::bounded(obj.Comment);
	jsn["addrandom"]=obj.addrandom;
	jsn["hideself"]=obj.hideself;
	jsn["wdog_timer"]=obj.wdog_timer;
	jsn["wdog1"]=obj.wdog1;
	jsn["wdog2"]=obj.wdog2;
	vector<string> ch_name_vec;
	ch_name_vec.resize(8);
	for(int i0=0;i0<8;i0++)
	{
		ch_name_vec[i0]=romana_bin_detail::bounded(obj.ch_name[i0]);
	}
	jsn["ch_name"]=ch_name_vec;
	jsn["drawopt"]=romana_bin_detail::bounded(obj.drawopt);
	jsn["ev_min"]=obj.ev_min;
	jsn["ev_max"]=obj.ev_max;
	jsn["buf_lag"]=obj.buf_lag;
	jsn["tgate"]=obj.tgate;
	jsn["tveto"]=obj.tveto;
	jsn["mult1"]=obj.mult1;
	jsn["mult2"]=obj.mult2;
	jsn["maintrig"]=obj.maintrig;
	jsn["seltab"]=obj.seltab;
	jsn["start_ch"]=obj.start_ch;
	jsn["ntof_period"]=obj.ntof_period;
	jsn["TofZero"]=obj.TofZero;
	jsn["Prof_type"]=obj.Prof_type;
	jsn["Ing_type"]=obj.Ing_type;
	jsn["prof_nx"]=obj.prof_nx;
	jsn["prof_ny"]=obj.prof_ny;
	vector<int> Ing_x_vec;
	Ing_x_vec.resize(16);
	for(int i0=0;i0<16;i0++)
	{
		Ing_x_vec[i0]=obj.Ing_x[i0];
	}
	jsn["Ing_x"]=Ing_x_vec;
	vector<int> Ing_y_vec;
	Ing_y_vec.resize(16);
	for(int i0=0;i0<16;i0++)
	{
		Ing_y_vec[i0]=obj.Ing_y[i0];
	}
	jsn["Ing_y"]=Ing_y_vec;
	vector<int> Prof_x_vec;
	Prof_x_vec.resize(8);
	for(int i0=0;i0<8;i0++)
	{
		Prof_x_vec[i0]=obj.Prof_x[i0];
	}
	jsn["Prof_x"]=Prof_x_vec;
	vector<int> Prof_y_vec;
	Prof_y_vec.resize(8);
	for(int i0=0;i0<8;i0++)
	{
		Prof_y_vec[i0]=obj.Prof_y[i0];
	}
	jsn["Prof_y"]=Prof_y_vec;
	vector<int> Prof64_vec;
	Prof64_vec.resize(5);
	for(int i0=0;i0<5;i0++)
	{
		Prof64_vec[i0]=obj.Prof64[i0];
	}
	jsn["Prof64"]=Prof64_vec;
	jsn["Prof64_TSP"]=romana_bin_detail::bounded(obj.Prof64_TSP);
	vector<int> Prof64_W_vec;
	Prof64_W_vec.resize(3);
	for(int i0=0;i0<3;i0++)
	{
		Prof64_W_vec[i0]=obj.Prof64_W[i0];
	}
	jsn["Prof64_W"]=Prof64_W_vec;
	jsn["Prof64_THR"]=obj.Prof64_THR;
	jsn["Prof64_GAT"]=obj.Prof64_GAT;
	jsn["Prof64_X"]=obj.Prof64_X;
	jsn["Prof64_Y"]=obj.Prof64_Y;
	jsn["num_events"]=obj.num_events;
	jsn["num_buf"]=obj.num_buf;
	jsn["b_logy"]=obj.b_logy;
	jsn["b_stat"]=obj.b_stat;
	jsn["b_gcuts"]=obj.b_gcuts;
	jsn["b_roi"]=obj.b_roi;
	jsn["b_fpeaks"]=obj.b_fpeaks;
	vector<bool> b_deriv_vec;
	b_deriv_vec.resize(5);
	for(int i0=0;i0<5;i0++)
	{
		b_deriv_vec[i0]=obj.b_deriv[i0];
	}
	jsn["b_deriv"]=b_deriv_vec;
	vector<bool> b_peak_vec;
	b_peak_vec.resize(14);
	for(int i0=0;i0<14;i0++)
	{
		b_peak_vec[i0]=obj.b_peak[i0];
	}
	jsn["b_peak"]=b_peak_vec;
	jsn["adcm_period"]=obj.adcm_period;
	jsn["ncuts"]=obj.ncuts;
	vector<int> pcuts_vec;
	pcuts_vec.resize(31);
	for(int i0=0;i0<31;i0++)
	{
		pcuts_vec[i0]=obj.pcuts[i0];
	}
	jsn["pcuts"]=pcuts_vec;
	vector<vector<vector<float> > > gcut_vec;
	gcut_vec.resize(31);
	for(int i0=0;i0<31;i0++)
	{
		gcut_vec[i0].resize(2);
		for(int i1=0;i1<2;i1++)
		{
			gcut_vec[i0][i1].resize(11);
			for(int i2=0;i2<11;i2++)
			{
				gcut_vec[i0][i1][i2]=obj.gcut[i0][i1][i2];
			}
		}
	}
	jsn["gcut"]=gcut_vec;
	vector<string> cut_form_vec;
	cut_form_vec.resize(31);
	for(int i0=0;i0<31;i0++)
	{
		cut_form_vec[i0]=obj.cut_form[i0];
	}
	jsn["cut_form"]=cut_form_vec;
	jsn["formula"]=romana_bin_detail::bounded(obj.formula);
	vector<float> SimSim_vec;
	SimSim_vec.resize(20);
	for(int i0=0;i0<20;i0++)
	{
		SimSim_vec[i0]=obj.SimSim[i0];
	}
	jsn["SimSim"]=SimSim_vec;
	jsn["Peak_thr"]=obj.Peak_thr;
	jsn["Peak_wid"]=obj.Peak_wid;
	jsn["Peak_bwidth"]=obj.Peak_bwidth;
	jsn["Peak_maxpeaks"]=obj.Peak_maxpeaks;
	jsn["Peak_smooth"]=obj.Peak_smooth;
	jsn["Peak_show_sm"]=obj.Peak_show_sm;
	jsn["Peak_use_mean"]=obj.Peak_use_mean;
	jsn["Peak_print"]=obj.Peak_print;
	vector<vector<int> > OptPar_vec;
	OptPar_vec.resize(15);
	for(int i0=0;i0<15;i0++)
	{
		OptPar_vec[i0].resize(4);
		for(int i1=0;i1<4;i1++)
		{
			OptPar_vec[i0][i1]=obj.OptPar[i0][i1];
		}
	}
	jsn["OptPar"]=OptPar_vec;
	vector<int> wrk_check_vec;
	wrk_check_vec.resize(128);
	for(int i0=0;i0<128;i0++)
	{
		wrk_check_vec[i0]=obj.wrk_check[i0];
	}
	jsn["wrk_check"]=wrk_check_vec;
	jsn["Nrows"]=obj.Nrows;
	jsn["ScrollPos"]=obj.ScrollPos;
	vector<float> hx_slider_vec;
	hx_slider_vec.resize(2);
	for(int i0=0;i0<2;i0++)
	{
		hx_slider_vec[i0]=obj.hx_slider[i0];
	}
	jsn["hx_slider"]=hx_slider_vec;
	vector<float> hy_slider_vec;
	hy_slider_vec.resize(2);
	for(int i0=0;i0<2;i0++)
	{
		hy_slider_vec[i0]=obj.hy_slider[i0];
	}
	jsn["hy_slider"]=hy_slider_vec;
}
