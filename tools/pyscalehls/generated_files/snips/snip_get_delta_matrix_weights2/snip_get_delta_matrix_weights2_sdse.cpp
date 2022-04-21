
//===------------------------------------------------------------*- C++ -*-===//
//
// Automatically generated file for High-level Synthesis (HLS).
//
//===----------------------------------------------------------------------===//

#include <algorithm>
#include <ap_axi_sdata.h>
#include <ap_fixed.h>
#include <ap_int.h>
#include <hls_math.h>
#include <hls_stream.h>
#include <math.h>
#include <stdint.h>

using namespace std;

/// This is top function.
/// Latency=73, interval=73
/// DSP=192
void get_delta_matrix_weights2(
  double v0[4096],
  double v1[64],
  double v2[64]
) {	// L5, [0,73)
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface bram port=v0
  #pragma HLS interface bram port=v2

  #pragma HLS array_partition variable=v0 cyclic factor=128 dim=1
  #pragma HLS resource variable=v0 core=ram_s2p_bram

  #pragma HLS array_partition variable=v1 cyclic factor=64 dim=1

  #pragma HLS array_partition variable=v2 cyclic factor=2 dim=1
  #pragma HLS resource variable=v2 core=ram_s2p_bram

  for (int v3 = 0; v3 < 32; v3 += 1) {	// L6, [0,71), iterCycle=7, II=2
    #pragma HLS pipeline II=2
    double v4 = v2[(v3 * 2)];	// L7, [0,2)
    double v5 = v1[0];	// L8, [0,2)
    double v6 = v4 * v5;	// L9, [2,6)
    v0[(v3 * 128)] = v6;	// L10, [6,7)
    double v7 = v1[1];	// L11, [0,2)
    double v8 = v4 * v7;	// L12, [2,6)
    v0[((v3 * 128) + 1)] = v8;	// L13, [6,7)
    double v9 = v1[2];	// L14, [0,2)
    double v10 = v4 * v9;	// L15, [2,6)
    v0[((v3 * 128) + 2)] = v10;	// L16, [6,7)
    double v11 = v1[3];	// L17, [0,2)
    double v12 = v4 * v11;	// L18, [2,6)
    v0[((v3 * 128) + 3)] = v12;	// L19, [6,7)
    double v13 = v1[4];	// L20, [0,2)
    double v14 = v4 * v13;	// L21, [2,6)
    v0[((v3 * 128) + 4)] = v14;	// L22, [6,7)
    double v15 = v1[5];	// L23, [0,2)
    double v16 = v4 * v15;	// L24, [2,6)
    v0[((v3 * 128) + 5)] = v16;	// L25, [6,7)
    double v17 = v1[6];	// L26, [0,2)
    double v18 = v4 * v17;	// L27, [2,6)
    v0[((v3 * 128) + 6)] = v18;	// L28, [6,7)
    double v19 = v1[7];	// L29, [0,2)
    double v20 = v4 * v19;	// L30, [2,6)
    v0[((v3 * 128) + 7)] = v20;	// L31, [6,7)
    double v21 = v1[8];	// L32, [0,2)
    double v22 = v4 * v21;	// L33, [2,6)
    v0[((v3 * 128) + 8)] = v22;	// L34, [6,7)
    double v23 = v1[9];	// L35, [0,2)
    double v24 = v4 * v23;	// L36, [2,6)
    v0[((v3 * 128) + 9)] = v24;	// L37, [6,7)
    double v25 = v1[10];	// L38, [0,2)
    double v26 = v4 * v25;	// L39, [2,6)
    v0[((v3 * 128) + 10)] = v26;	// L40, [6,7)
    double v27 = v1[11];	// L41, [0,2)
    double v28 = v4 * v27;	// L42, [2,6)
    v0[((v3 * 128) + 11)] = v28;	// L43, [6,7)
    double v29 = v1[12];	// L44, [0,2)
    double v30 = v4 * v29;	// L45, [2,6)
    v0[((v3 * 128) + 12)] = v30;	// L46, [6,7)
    double v31 = v1[13];	// L47, [0,2)
    double v32 = v4 * v31;	// L48, [2,6)
    v0[((v3 * 128) + 13)] = v32;	// L49, [6,7)
    double v33 = v1[14];	// L50, [0,2)
    double v34 = v4 * v33;	// L51, [2,6)
    v0[((v3 * 128) + 14)] = v34;	// L52, [6,7)
    double v35 = v1[15];	// L53, [0,2)
    double v36 = v4 * v35;	// L54, [2,6)
    v0[((v3 * 128) + 15)] = v36;	// L55, [6,7)
    double v37 = v1[16];	// L56, [0,2)
    double v38 = v4 * v37;	// L57, [2,6)
    v0[((v3 * 128) + 16)] = v38;	// L58, [6,7)
    double v39 = v1[17];	// L59, [0,2)
    double v40 = v4 * v39;	// L60, [2,6)
    v0[((v3 * 128) + 17)] = v40;	// L61, [6,7)
    double v41 = v1[18];	// L62, [0,2)
    double v42 = v4 * v41;	// L63, [2,6)
    v0[((v3 * 128) + 18)] = v42;	// L64, [6,7)
    double v43 = v1[19];	// L65, [0,2)
    double v44 = v4 * v43;	// L66, [2,6)
    v0[((v3 * 128) + 19)] = v44;	// L67, [6,7)
    double v45 = v1[20];	// L68, [0,2)
    double v46 = v4 * v45;	// L69, [2,6)
    v0[((v3 * 128) + 20)] = v46;	// L70, [6,7)
    double v47 = v1[21];	// L71, [0,2)
    double v48 = v4 * v47;	// L72, [2,6)
    v0[((v3 * 128) + 21)] = v48;	// L73, [6,7)
    double v49 = v1[22];	// L74, [0,2)
    double v50 = v4 * v49;	// L75, [2,6)
    v0[((v3 * 128) + 22)] = v50;	// L76, [6,7)
    double v51 = v1[23];	// L77, [0,2)
    double v52 = v4 * v51;	// L78, [2,6)
    v0[((v3 * 128) + 23)] = v52;	// L79, [6,7)
    double v53 = v1[24];	// L80, [0,2)
    double v54 = v4 * v53;	// L81, [2,6)
    v0[((v3 * 128) + 24)] = v54;	// L82, [6,7)
    double v55 = v1[25];	// L83, [0,2)
    double v56 = v4 * v55;	// L84, [2,6)
    v0[((v3 * 128) + 25)] = v56;	// L85, [6,7)
    double v57 = v1[26];	// L86, [0,2)
    double v58 = v4 * v57;	// L87, [2,6)
    v0[((v3 * 128) + 26)] = v58;	// L88, [6,7)
    double v59 = v1[27];	// L89, [0,2)
    double v60 = v4 * v59;	// L90, [2,6)
    v0[((v3 * 128) + 27)] = v60;	// L91, [6,7)
    double v61 = v1[28];	// L92, [0,2)
    double v62 = v4 * v61;	// L93, [2,6)
    v0[((v3 * 128) + 28)] = v62;	// L94, [6,7)
    double v63 = v1[29];	// L95, [0,2)
    double v64 = v4 * v63;	// L96, [2,6)
    v0[((v3 * 128) + 29)] = v64;	// L97, [6,7)
    double v65 = v1[30];	// L98, [0,2)
    double v66 = v4 * v65;	// L99, [2,6)
    v0[((v3 * 128) + 30)] = v66;	// L100, [6,7)
    double v67 = v1[31];	// L101, [0,2)
    double v68 = v4 * v67;	// L102, [2,6)
    v0[((v3 * 128) + 31)] = v68;	// L103, [6,7)
    double v69 = v1[32];	// L104, [0,2)
    double v70 = v4 * v69;	// L105, [2,6)
    v0[((v3 * 128) + 32)] = v70;	// L106, [6,7)
    double v71 = v1[33];	// L107, [0,2)
    double v72 = v4 * v71;	// L108, [2,6)
    v0[((v3 * 128) + 33)] = v72;	// L109, [6,7)
    double v73 = v1[34];	// L110, [0,2)
    double v74 = v4 * v73;	// L111, [2,6)
    v0[((v3 * 128) + 34)] = v74;	// L112, [6,7)
    double v75 = v1[35];	// L113, [0,2)
    double v76 = v4 * v75;	// L114, [2,6)
    v0[((v3 * 128) + 35)] = v76;	// L115, [6,7)
    double v77 = v1[36];	// L116, [0,2)
    double v78 = v4 * v77;	// L117, [2,6)
    v0[((v3 * 128) + 36)] = v78;	// L118, [6,7)
    double v79 = v1[37];	// L119, [0,2)
    double v80 = v4 * v79;	// L120, [2,6)
    v0[((v3 * 128) + 37)] = v80;	// L121, [6,7)
    double v81 = v1[38];	// L122, [0,2)
    double v82 = v4 * v81;	// L123, [2,6)
    v0[((v3 * 128) + 38)] = v82;	// L124, [6,7)
    double v83 = v1[39];	// L125, [0,2)
    double v84 = v4 * v83;	// L126, [2,6)
    v0[((v3 * 128) + 39)] = v84;	// L127, [6,7)
    double v85 = v1[40];	// L128, [0,2)
    double v86 = v4 * v85;	// L129, [2,6)
    v0[((v3 * 128) + 40)] = v86;	// L130, [6,7)
    double v87 = v1[41];	// L131, [0,2)
    double v88 = v4 * v87;	// L132, [2,6)
    v0[((v3 * 128) + 41)] = v88;	// L133, [6,7)
    double v89 = v1[42];	// L134, [0,2)
    double v90 = v4 * v89;	// L135, [2,6)
    v0[((v3 * 128) + 42)] = v90;	// L136, [6,7)
    double v91 = v1[43];	// L137, [0,2)
    double v92 = v4 * v91;	// L138, [2,6)
    v0[((v3 * 128) + 43)] = v92;	// L139, [6,7)
    double v93 = v1[44];	// L140, [0,2)
    double v94 = v4 * v93;	// L141, [2,6)
    v0[((v3 * 128) + 44)] = v94;	// L142, [6,7)
    double v95 = v1[45];	// L143, [0,2)
    double v96 = v4 * v95;	// L144, [2,6)
    v0[((v3 * 128) + 45)] = v96;	// L145, [6,7)
    double v97 = v1[46];	// L146, [0,2)
    double v98 = v4 * v97;	// L147, [2,6)
    v0[((v3 * 128) + 46)] = v98;	// L148, [6,7)
    double v99 = v1[47];	// L149, [0,2)
    double v100 = v4 * v99;	// L150, [2,6)
    v0[((v3 * 128) + 47)] = v100;	// L151, [6,7)
    double v101 = v1[48];	// L152, [0,2)
    double v102 = v4 * v101;	// L153, [2,6)
    v0[((v3 * 128) + 48)] = v102;	// L154, [6,7)
    double v103 = v1[49];	// L155, [0,2)
    double v104 = v4 * v103;	// L156, [2,6)
    v0[((v3 * 128) + 49)] = v104;	// L157, [6,7)
    double v105 = v1[50];	// L158, [0,2)
    double v106 = v4 * v105;	// L159, [2,6)
    v0[((v3 * 128) + 50)] = v106;	// L160, [6,7)
    double v107 = v1[51];	// L161, [0,2)
    double v108 = v4 * v107;	// L162, [2,6)
    v0[((v3 * 128) + 51)] = v108;	// L163, [6,7)
    double v109 = v1[52];	// L164, [0,2)
    double v110 = v4 * v109;	// L165, [2,6)
    v0[((v3 * 128) + 52)] = v110;	// L166, [6,7)
    double v111 = v1[53];	// L167, [0,2)
    double v112 = v4 * v111;	// L168, [2,6)
    v0[((v3 * 128) + 53)] = v112;	// L169, [6,7)
    double v113 = v1[54];	// L170, [0,2)
    double v114 = v4 * v113;	// L171, [2,6)
    v0[((v3 * 128) + 54)] = v114;	// L172, [6,7)
    double v115 = v1[55];	// L173, [0,2)
    double v116 = v4 * v115;	// L174, [2,6)
    v0[((v3 * 128) + 55)] = v116;	// L175, [6,7)
    double v117 = v1[56];	// L176, [0,2)
    double v118 = v4 * v117;	// L177, [2,6)
    v0[((v3 * 128) + 56)] = v118;	// L178, [6,7)
    double v119 = v1[57];	// L179, [0,2)
    double v120 = v4 * v119;	// L180, [2,6)
    v0[((v3 * 128) + 57)] = v120;	// L181, [6,7)
    double v121 = v1[58];	// L182, [0,2)
    double v122 = v4 * v121;	// L183, [2,6)
    v0[((v3 * 128) + 58)] = v122;	// L184, [6,7)
    double v123 = v1[59];	// L185, [0,2)
    double v124 = v4 * v123;	// L186, [2,6)
    v0[((v3 * 128) + 59)] = v124;	// L187, [6,7)
    double v125 = v1[60];	// L188, [0,2)
    double v126 = v4 * v125;	// L189, [2,6)
    v0[((v3 * 128) + 60)] = v126;	// L190, [6,7)
    double v127 = v1[61];	// L191, [0,2)
    double v128 = v4 * v127;	// L192, [2,6)
    v0[((v3 * 128) + 61)] = v128;	// L193, [6,7)
    double v129 = v1[62];	// L194, [0,2)
    double v130 = v4 * v129;	// L195, [2,6)
    v0[((v3 * 128) + 62)] = v130;	// L196, [6,7)
    double v131 = v1[63];	// L197, [0,2)
    double v132 = v4 * v131;	// L198, [2,6)
    v0[((v3 * 128) + 63)] = v132;	// L199, [6,7)
    double v133 = v2[((v3 * 2) + 1)];	// L200, [0,2)
    double v134 = v133 * v5;	// L201, [2,6)
    v0[((v3 * 128) + 64)] = v134;	// L202, [6,7)
    double v135 = v133 * v7;	// L203, [2,6)
    v0[((v3 * 128) + 65)] = v135;	// L204, [6,7)
    double v136 = v133 * v9;	// L205, [2,6)
    v0[((v3 * 128) + 66)] = v136;	// L206, [6,7)
    double v137 = v133 * v11;	// L207, [2,6)
    v0[((v3 * 128) + 67)] = v137;	// L208, [6,7)
    double v138 = v133 * v13;	// L209, [2,6)
    v0[((v3 * 128) + 68)] = v138;	// L210, [6,7)
    double v139 = v133 * v15;	// L211, [2,6)
    v0[((v3 * 128) + 69)] = v139;	// L212, [6,7)
    double v140 = v133 * v17;	// L213, [2,6)
    v0[((v3 * 128) + 70)] = v140;	// L214, [6,7)
    double v141 = v133 * v19;	// L215, [2,6)
    v0[((v3 * 128) + 71)] = v141;	// L216, [6,7)
    double v142 = v133 * v21;	// L217, [2,6)
    v0[((v3 * 128) + 72)] = v142;	// L218, [6,7)
    double v143 = v133 * v23;	// L219, [2,6)
    v0[((v3 * 128) + 73)] = v143;	// L220, [6,7)
    double v144 = v133 * v25;	// L221, [2,6)
    v0[((v3 * 128) + 74)] = v144;	// L222, [6,7)
    double v145 = v133 * v27;	// L223, [2,6)
    v0[((v3 * 128) + 75)] = v145;	// L224, [6,7)
    double v146 = v133 * v29;	// L225, [2,6)
    v0[((v3 * 128) + 76)] = v146;	// L226, [6,7)
    double v147 = v133 * v31;	// L227, [2,6)
    v0[((v3 * 128) + 77)] = v147;	// L228, [6,7)
    double v148 = v133 * v33;	// L229, [2,6)
    v0[((v3 * 128) + 78)] = v148;	// L230, [6,7)
    double v149 = v133 * v35;	// L231, [2,6)
    v0[((v3 * 128) + 79)] = v149;	// L232, [6,7)
    double v150 = v133 * v37;	// L233, [2,6)
    v0[((v3 * 128) + 80)] = v150;	// L234, [6,7)
    double v151 = v133 * v39;	// L235, [2,6)
    v0[((v3 * 128) + 81)] = v151;	// L236, [6,7)
    double v152 = v133 * v41;	// L237, [2,6)
    v0[((v3 * 128) + 82)] = v152;	// L238, [6,7)
    double v153 = v133 * v43;	// L239, [2,6)
    v0[((v3 * 128) + 83)] = v153;	// L240, [6,7)
    double v154 = v133 * v45;	// L241, [2,6)
    v0[((v3 * 128) + 84)] = v154;	// L242, [6,7)
    double v155 = v133 * v47;	// L243, [2,6)
    v0[((v3 * 128) + 85)] = v155;	// L244, [6,7)
    double v156 = v133 * v49;	// L245, [2,6)
    v0[((v3 * 128) + 86)] = v156;	// L246, [6,7)
    double v157 = v133 * v51;	// L247, [2,6)
    v0[((v3 * 128) + 87)] = v157;	// L248, [6,7)
    double v158 = v133 * v53;	// L249, [2,6)
    v0[((v3 * 128) + 88)] = v158;	// L250, [6,7)
    double v159 = v133 * v55;	// L251, [2,6)
    v0[((v3 * 128) + 89)] = v159;	// L252, [6,7)
    double v160 = v133 * v57;	// L253, [2,6)
    v0[((v3 * 128) + 90)] = v160;	// L254, [6,7)
    double v161 = v133 * v59;	// L255, [2,6)
    v0[((v3 * 128) + 91)] = v161;	// L256, [6,7)
    double v162 = v133 * v61;	// L257, [2,6)
    v0[((v3 * 128) + 92)] = v162;	// L258, [6,7)
    double v163 = v133 * v63;	// L259, [2,6)
    v0[((v3 * 128) + 93)] = v163;	// L260, [6,7)
    double v164 = v133 * v65;	// L261, [2,6)
    v0[((v3 * 128) + 94)] = v164;	// L262, [6,7)
    double v165 = v133 * v67;	// L263, [2,6)
    v0[((v3 * 128) + 95)] = v165;	// L264, [6,7)
    double v166 = v133 * v69;	// L265, [2,6)
    v0[((v3 * 128) + 96)] = v166;	// L266, [6,7)
    double v167 = v133 * v71;	// L267, [2,6)
    v0[((v3 * 128) + 97)] = v167;	// L268, [6,7)
    double v168 = v133 * v73;	// L269, [2,6)
    v0[((v3 * 128) + 98)] = v168;	// L270, [6,7)
    double v169 = v133 * v75;	// L271, [2,6)
    v0[((v3 * 128) + 99)] = v169;	// L272, [6,7)
    double v170 = v133 * v77;	// L273, [2,6)
    v0[((v3 * 128) + 100)] = v170;	// L274, [6,7)
    double v171 = v133 * v79;	// L275, [2,6)
    v0[((v3 * 128) + 101)] = v171;	// L276, [6,7)
    double v172 = v133 * v81;	// L277, [2,6)
    v0[((v3 * 128) + 102)] = v172;	// L278, [6,7)
    double v173 = v133 * v83;	// L279, [2,6)
    v0[((v3 * 128) + 103)] = v173;	// L280, [6,7)
    double v174 = v133 * v85;	// L281, [2,6)
    v0[((v3 * 128) + 104)] = v174;	// L282, [6,7)
    double v175 = v133 * v87;	// L283, [2,6)
    v0[((v3 * 128) + 105)] = v175;	// L284, [6,7)
    double v176 = v133 * v89;	// L285, [2,6)
    v0[((v3 * 128) + 106)] = v176;	// L286, [6,7)
    double v177 = v133 * v91;	// L287, [2,6)
    v0[((v3 * 128) + 107)] = v177;	// L288, [6,7)
    double v178 = v133 * v93;	// L289, [2,6)
    v0[((v3 * 128) + 108)] = v178;	// L290, [6,7)
    double v179 = v133 * v95;	// L291, [2,6)
    v0[((v3 * 128) + 109)] = v179;	// L292, [6,7)
    double v180 = v133 * v97;	// L293, [2,6)
    v0[((v3 * 128) + 110)] = v180;	// L294, [6,7)
    double v181 = v133 * v99;	// L295, [2,6)
    v0[((v3 * 128) + 111)] = v181;	// L296, [6,7)
    double v182 = v133 * v101;	// L297, [2,6)
    v0[((v3 * 128) + 112)] = v182;	// L298, [6,7)
    double v183 = v133 * v103;	// L299, [2,6)
    v0[((v3 * 128) + 113)] = v183;	// L300, [6,7)
    double v184 = v133 * v105;	// L301, [2,6)
    v0[((v3 * 128) + 114)] = v184;	// L302, [6,7)
    double v185 = v133 * v107;	// L303, [2,6)
    v0[((v3 * 128) + 115)] = v185;	// L304, [6,7)
    double v186 = v133 * v109;	// L305, [2,6)
    v0[((v3 * 128) + 116)] = v186;	// L306, [6,7)
    double v187 = v133 * v111;	// L307, [2,6)
    v0[((v3 * 128) + 117)] = v187;	// L308, [6,7)
    double v188 = v133 * v113;	// L309, [2,6)
    v0[((v3 * 128) + 118)] = v188;	// L310, [6,7)
    double v189 = v133 * v115;	// L311, [2,6)
    v0[((v3 * 128) + 119)] = v189;	// L312, [6,7)
    double v190 = v133 * v117;	// L313, [2,6)
    v0[((v3 * 128) + 120)] = v190;	// L314, [6,7)
    double v191 = v133 * v119;	// L315, [2,6)
    v0[((v3 * 128) + 121)] = v191;	// L316, [6,7)
    double v192 = v133 * v121;	// L317, [2,6)
    v0[((v3 * 128) + 122)] = v192;	// L318, [6,7)
    double v193 = v133 * v123;	// L319, [2,6)
    v0[((v3 * 128) + 123)] = v193;	// L320, [6,7)
    double v194 = v133 * v125;	// L321, [2,6)
    v0[((v3 * 128) + 124)] = v194;	// L322, [6,7)
    double v195 = v133 * v127;	// L323, [2,6)
    v0[((v3 * 128) + 125)] = v195;	// L324, [6,7)
    double v196 = v133 * v129;	// L325, [2,6)
    v0[((v3 * 128) + 126)] = v196;	// L326, [6,7)
    double v197 = v133 * v131;	// L327, [2,6)
    v0[((v3 * 128) + 127)] = v197;	// L328, [6,7)
  }
}

