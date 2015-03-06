
// The Messages



	

		// fixed int64 size message.
		
		#define 	FILE_VERSION 	1000
		#define	SONG_START		1050
		#define	SONG_STOP		1051
		#define	SONG_TIME		1070
		#define	SONG_NUM_NOTES	1073
		#define	SONG_POPUP_DESC	1072
		#define	SONG_PLAY_ALL	1071
		#define	SONG_CUTOFF	1080
		#define	SONG_RESONANCE	1081
		#define	SONG_FILTER_ON	1082
		
		#define	SONG_VST_START	1090
		#define	SONG_VST_STOP	1091
		#define	SONG_VST_ON	1092
		#define	SONG_TRACKBOOST	1093
				
		#define	PATTERN_START	1100
		#define	PATTERN_STOP	1101	
		
		#define	TRACK_START	1200
		#define	TRACK_STOP		1201
		
		#define	TRACK_VOL		1206
		#define	TRACK_PAN		1208
		#define	TRACK_NAME_SET	2209
		#define	TRACK_MUTE		1211
		#define	TRACK_CIS		1213	
		#define	TRACK_TYPE		1214
		#define	TRACK_REVERSED	1215
		#define	TRACK_FX1		1216
		#define	TRACK_FX1_LINE	1217
		#define	TRACK_RESAMPLE_EN	1218
		#define	TRACK_RESAMPLE		1219
		#define	TRACK_AMPBOOST			1220
		
		#define	NOTE_START		1300
		#define	NOTE_STOP		1301
		#define	NOTE_ON		1302
		#define	NOTE_VOLUME	1303
		#define	NOTE_OCT		1304	//old before 1.2
		#define	NOTE_NOTE		1305	//old before 1.2
		#define	NOTE_NEWNOTE	1306
		
		#define	EXT_LIST_START	1400
		#define	EXT_LIST_STOP	1401
		
		// play list section..
		#define	PL_START		1500
		#define	PL_STOP		1501
		#define	PL_POS_START	1510
		#define	PL_POS_STOP		1511
		#define	PL_POS_PAT		1512
			
		
		// variable size message
		#define	TRACK_SAMPLE	2202
		#define	TRACK_NAME		2203
		#define	FILE_NOTE		2024
		#define	BANK_NAME		2030	
		#define	EXT_SAMPLE_PATH	2040
		#define	SONG_TITLE		2075
		#define	SONG_DESC		2076
		#define	SONG_VST_NAME	2077
		#define	SONG_VST_DATA	2078
		#define	SONG_VST_LINE	2079	
		#define	SONG_VST_GLOBAL	2080	
		//message like style >3000
		
		#define	TRACK_SPECIFIC_DATA_START 3000
		//#define	TRACK_SPECIFIC_DATA_STOP   3001 //useless!!
	

		
	

	

		
	
	