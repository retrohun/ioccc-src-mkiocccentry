/* vim: set tabstop=8 softtabstop=4 shiftwidth=4 noexpandtab : */
/*
 * foo - bar
 *
 * "Because sometimes code is foo and sometimes foo is fubar." :-)
 *
 * The JSON parser was co-developed by:
 *
 *	@xexyl
 *	https://xexyl.net		Cody Boone Ferguson
 *	https://ioccc.xexyl.net
 * and:
 *	chongo (Landon Curt Noll, http://www.isthe.com/chongo/index.html) /\oo/\
 *
 * "Because sometimes we need more O in C." :-)
 *
 * This tool is being co-developed by Cody Boone Ferguson and Landon Curt Noll.
 *
 * The concept of this file was developed by:
 *
 *	Cody Boone Ferguson
 *
 * after some discussion with Landon Curt Noll about the need (yes: need) for
 * it. Cody is quite okay if after looking at this file you think he's a bit
 * crazy and in fact he quite welcomes it and even relishes the idea! :-)
 *
 * "Because sometimes we all need a little craziness in our lives." :-)
 * "Because sometimes even the IOCCC Judges need some help." :-)
 */


/* special comments for the seqcexit tool */
/*ooo*/ /* exit code out of numerical order - ignore in sequencing */
/*coo*/ /* exit code change of order - use new value in sequencing */


#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

/*
 * foo - bar
 */
#include "foo.h"


/*
 * static values
 */

/* Special stuff you're not supposed to not know about :-( .. and more stuff :-) */
static time_t t = 314159223+42;
static char const *oebxergfB[] =
{
    "'yrff' vf yrkvpbtencuvpnyyl terngre guna 'terngre'. '<', ubjrire,\n"
    "vf yrkvpbtencuvpnyyl yrff guna '>'. Fbzrbar fperjrq hc.\n-- Krkly",
    "V qba'g xabj unys bs lbh unys nf jryy nf V fubhyq yvxr;\nnaq V yvxr yrff guna"
    " unys bs lbh unys nf jryy nf lbh qrfreir.\n-- Ovyob Onttvaf",
    "P frpgvba: '{}'\n-- Krkly\n",
    "'Frpbaqyl, gb pryroengr BHE oveguqnlf: zvar naq zl ubabhenoyr naq tnyynag\n"
    "sngure'f.' Hapbzsbegnoyr naq nccerurafvir fvyrapr. 'V nz bayl unys gur zna\n"
    "gung ur vf: V nz 72, ur vf 144. Lbhe ahzoref ner pubfra gb qb ubabhe gb\n"
    "rnpu bs uvf ubabhenoyr lrnef.' Guvf jnf ernyyl qernqshy -- n erthyne\n"
    "oenvagjvfgre, naq fbzr bs gurz sryg vafhygrq, yvxr yrnc-qnlf fubirq\n"
    "va gb svyy hc n pnyraqne.\n-- Ovatb Onttvaf",
    "P havbaf: ybivat pbhcyrf jub rawbl cebtenzzvat va P gbtrgure.\n-- Krkly\n",
    "'Ybbx,' fnvq Neguhe, 'jbhyq vg fnir lbh n ybg bs gvzr vs V whfg tnir\n"
    "hc naq jrag znq abj?'\n",
    "Gurer vf na neg, be, engure, n xanpx gb sylvat. Gur xanpx yvrf va\n"
    "yrneavat ubj gb guebj lbhefrys ng gur tebhaq naq zvff.\n",
    "SHA SNPG: Va O naq rneyl P, gur += bcrengbe jnf fcryg =+ vafgrnq bs += ;\n"
    "guvf zvfgnxr, ercnverq va 1976, jnf vaqhprq ol n frqhpgviryl rnfl jnl bs\n"
    "unaqyvat gur svefg sbez va O'f yrkvpny nanylfre.\n\nFrr zber sha snpgf naq"
    " uvfgbel ol Qraavf Evgpuvr uvzfrys ng\nuggcf://jjj.oryy-ynof.pbz/hfe/qze/jjj/puvfg.ugzy.\n",
    "'Jung vf vg?' tebjyrq Fnz, zvfvagrecergvat gur fvtaf. 'Jung'f gur arrq gb\n"
    "favss? Gur fgvax arneyl xabpxf zr qbja jvgu zl abfr uryq. Lbh fgvax, naq znfgre\n"
    "fgvaxf; gur jubyr cynpr fgvaxf.'\n\n'Lrf, lrf, naq Fnz fgvaxf!' nafjrerq Tbyyhz. "
    "'Cbbe Fzéntby fzryyf vg,\nohg tbbq Fzéntby ornef vg. Urycf avpr znfgre. Ohg gung'f"
    " ab znggre. Gur nve'f\nzbivat, punatr vf pbzvat. Fzéntby jbaqref; ur'f abg unccl.'\n",
    "'Anl! Abg Ryirf,' fnvq gur sbhegu, gur gnyyrfg, naq nf vg nccrnerq gur\npuvrs nzbat"
    " gurz. 'Ryirf qb abg jnyx va Vguvyvra va gurfr qnlf. Naq\nRyirf ner jbaqebhf snve gb"
    " ybbx hcba, be fb 'gvf fnvq.'\n\n'Zrnavat jr'er abg, V gnxr lbh,' fnvq Fnz. 'Gunax"
    " lbh xvaqyl. Naq jura\nlbh'ir svavfurq qvfphffvat hf, creuncf lbh'yy fnl jub lbh ner,"
    " naq\njul lbh pna'g yrg gjb gverq geniryyref erfg.'\n",
    "Tvyqbe jnf fvyrag sbe n zbzrag. 'V qb abg yvxr guvf arjf,' ur fnvq\n"
    "ng ynfg. 'Gung Tnaqnys fubhyq or yngr, qbrf abg obqr jryy. Ohg vg\nvf fnvq:"
    " Qb abg zrqqyr va gur nssnvef bs Jvmneqf, sbe gurl ner\nfhogyr naq dhvpx gb "
    "natre. Gur pubvpr vf lbhef: gb tb be jnvg.'\n\n'Naq vg vf nyfb fnvq,' nafjrerq"
    " Sebqb: 'Tb abg gb gur Ryirf sbe\npbhafry, sbe gurl jvyy fnl obgu ab naq lrf.'",
    "Bar bs gur guvatf Sbeq Cersrpg unq nyjnlf sbhaq uneqrfg gb haqrefgnaq nobhg\n"
    "uhznaf jnf gurve unovg bs pbagvahnyyl fgngvat naq ercrngvat gur irel irel\n"
    "boivbhf.\n\nBar bs gur guvatf Sbeq Cersrpg unq nyjnlf sbhaq uneqrfg gb haqrefgnaq nobhg\n"
    "uhznaf jnf gurve unovg bs pbagvahnyyl fgngvat naq ercrngvat gur irel irel\nboivbhf.",
    "Sbe n zbzrag, abguvat unccrarq. Gura, nsgre n frpbaq be fb, abguvat\npbagvahrq gb unccra.",
    "'Vs V rire zrrg zlfrys,' fnvq Mncubq, 'V'yy uvg zlfrys fb uneq V jba'g xabj\njung'f uvg zr.'",
    "'V frr abobql ba gur ebnq,' fnvq Nyvpr.\n\n'V bayl jvfu gung V unq fhpu rlrf,' gur Xvat erznexrq"
    " va n sergshy\ngbar. 'Gb or noyr gb frr Abobql! Naq ng gung qvfgnapr gbb! Jul, vg'f\nnf zhpu nf V"
    " pna qb gb frr erny crbcyr, ol guvf yvtug!'\n\n[...]\n\n'Jub qvq lbh cnff ba gur ebnq?' gur Xvat jrag"
    " ba, ubyqvat bhg uvf\nunaq gb gur Zrffratre sbe fbzr zber unl.\n\n'Abobql,' fnvq gur Zrffratre.\n\n'"
    "Dhvgr evtug,' fnvq gur Xvat: 'guvf lbhat ynql fnj uvz gbb. Fb bs pbhefr Abobql\njnyxf fybjre guna lbh.'\n\n"
    "'V qb zl orfg,' gur Zrffratre fnvq va n fhyxl gbar. 'V'z fher abobql jnyxf zhpu\nsnfgre guna V qb!'\n\n"
    "'Ur pna'g qb gung,' fnvq gur Xvat, 'be ryfr ur'q unir orra urer svefg.'",
    "'Guvf vf n puvyq!' Unvtun ercyvrq rntreyl, pbzvat va sebag bs Nyvpr gb\n"
    "vagebqhpr ure, naq fcernqvat bhg obgu uvf unaqf gbjneqf ure va na Natyb-Fnkba\n"
    "nggvghqr. 'Jr bayl sbhaq vg gb-qnl. Vg'f nf ynetr nf yvsr, naq gjvpr nf\nangheny!'\n\n"
    "'V nyjnlf gubhtug gurl jrer snohybhf zbafgref!' fnvq gur Havpbea. 'Vf vg\nnyvir?'\n\n'"
    "Vg pna gnyx,' fnvq Unvtun, fbyrzayl.\nGur Havpbea ybbxrq qernzvyl ng Nyvpr, naq fnvq 'Gnyx, puvyq.'\n"
    "Nyvpr pbhyq abg uryc ure yvcf pheyvat hc vagb n fzvyr nf fur ortna: 'Qb lbh\nxabj, V nyjnlf gubhtug"
    " Havpbeaf jrer snohybhf zbafgref, gbb! V arire fnj bar\nnyvir orsber!'\n\n[...]\n\n'Nu, jung vf vg, abj?'"
    " gur Havpbea pevrq rntreyl. 'Lbh'yy arire thrff! V\npbhyqa'g.' Gur Yvba ybbxrq ng Nyvpr jrnevyl. 'Ner lbh"
    " na navzny -- irtrgnoyr --\nbe zvareny?' ur fnvq, lnjavat ng rirel bgure jbeq.\n\n'Vg'f n snohybhf zbafgre!'"
    " gur Havpbea pevrq bhg, orsber Nyvpr pbhyq ercyl.\n\n'Gura unaq ebhaq gur cyhz-pnxr, Zbafgre,' gur Yvba fnvq.",
    "V ortna gb unir qbhogf nobhg ubj zngu vf gnhtug va guvf pbhagel jura n lbhat\n"
    "pbbx ng zl cvmmrevn fnvq ur unq n ceboyrz. 'N phfgbzre pnyyrq, nfxvat sbe n\ncvr gung'f n"
    " guveq purrfr, n guveq crccrebav, naq n guveq pbzovangvba,' ur\nfnvq. 'Fb, jung'f gur ceboyrz?' "
    "V nfxrq uvz. 'V qba'g xabj jung gb chg ba\ngur sbhegu guveq'.\n\n-EBOREG XRNEARL\nEnapub Pbeqbin,\nPnyvsbeavn",
    "'Naq lbh qb Nqqvgvba?' gur Juvgr Dhrra nfxrq. 'Jung'f bar naq bar naq bar naq\nbar naq bar naq bar"
    " naq bar naq bar naq bar naq bar?' 'V qba'g xabj,' fnvq\nNyvpr. 'V ybfg pbhag.'\n\n'Fur pna'g qb Nqqvgvba,'"
    " gur Erq Dhrra vagreehcgrq. 'Pna lbh qb Fhogenpgvba?'\nGnxr avar sebz rvtug.' 'Avar sebz rvtug V pna'g, lbh xabj,'"
    " Nyvpr ercyvrq irel\nernqvyl: 'ohg--'\n\n'Fur pna'g qb Fhogenpgvba,' fnvq gur Juvgr Dhrra. 'Pna lbh qb Qvivfvba? "
    "Qvivqr\nn ybns ol n xavsr -- jung'f gur nafjre gb gung?' 'V fhccbfr--' Nyvpr jnf\nortvaavat, ohg gur Erq Dhrra "
    "nafjrerq sbe ure.  'Oernq-naq-ohggre, bs pbhefr.\nGel nabgure Fhogenpgvba fhz. Gnxr n obar sebz n qbt: jung erznvaf?'\n\n"
    "Nyvpr pbafvqrerq. 'Gur obar jbhyqa'g erznva, bs pbhefr, vs V gbbx vg--naq gur\nqbt jbhyqa'g erznva; vg jbhyq pbzr gb ovgr "
    "zr--naq V'z fher V fubhyqa'g\nerznva!' 'Gura lbh guvax abguvat jbhyq erznva?' fnvq gur Erq Dhrra.\n\n'V guvax gung'f gur "
    "nafjre.' 'Jebat, nf hfhny,' fnvq gur Erq Dhrra: 'gur qbt'f\ngrzcre jbhyq erznva.' 'Ohg V qba'g frr ubj --'\n\n"
    "'Jul, ybbx urer!' gur Erq Dhrra pevrq. 'Gur qbt jbhyq ybfr vgf grzcre, jbhyqa'g\nvg?'\n\n"
    "'Creuncf vg jbhyq,' Nyvpr ercyvrq pnhgvbhfyl. 'Gura vs gur qbt jrag njnl, vgf\ngrzcre jbhyq erznva!' "
    "gur Dhrra rkpynvzrq gevhzcunagyl.",
    "Urer gur Erq Dhrra ortna ntnva. 'Pna lbh nafjre hfrshy dhrfgvbaf?' fur fnvq.\n"
    "'Ubj vf oernq znqr?'\n\n'V xabj gung!' Nyvpr pevrq rntreyl.' Lbh gnxr fbzr sybhe--'\n"
    "'Jurer qb lbh cvpx gur sybjre?' gur Juvgr Dhrra nfxrq. 'Va n tneqra, be va gur\nurqtrf?'\n\n"
    "'Jryy vg vfa'g cvpxrq ng nyy,' Nyvpr rkcynvarq: 'vg'f tebhaq--'\n'Ubj znal nperf bs tebhaq?' "
    "fnvq gur Juvgr Dhrra. 'Lbh zhfga'g yrnir bhg fb\nznal guvatf.'\n\n'Sna ure urnq!' gur Erq Dhrra "
    "nakvbhfyl vagreehcgrq. 'Fur'yy or srirevfu nsgre\nfb zhpu guvaxvat.' Fb gurl frg gb jbex naq snaarq "
    "ure jvgu ohapurf bs yrnirf,\ngvyy fur unq gb ort gurz gb yrnir bss, vg oyrj ure unve nobhg fb.",
    "SHA SNPG:\n\nVa Yvsr, gur Havirefr naq Rirelguvat, gur guveq vafgnyzrag bs Gur Uvgpuuvxre'f\n"
    "Thvqr gb gur Tnynkl ol Qbhtynf Nqnzf, vg'f fnvq gur nafjre gb Yvsr, gur\nHavirefr naq Rirelguvat vf 42. "
    "Qbhtynf Nqnzf vafvfgrq guvf jnf n wbxr, jevgvat\nba HFRARG:\n\n    Gur nafjre gb guvf vf irel fvzcyr. Vg "
    "jnf n wbxr. Vg unq gb or n ahzore, na\n    beqvanel, fznyyvfu ahzore, naq V pubfr gung bar. Ovanel ercerfragngvbaf,"
    "\n    onfr guvegrra, Gvorgna zbaxf ner nyy pbzcyrgr abafrafr. V fng ng zl qrfx,\n    fgnerq vagb gur tneqra naq gubhtug"
    " '42 jvyy qb' V glcrq vg bhg. Raq bs\n    fgbel.\n\nOhg qvq lbh xabj gung fbzr nfgebabzref FCRAG GJB LRNEF fghqlvat guvf "
    "naq VAFVFG\ngung 42 vf npghnyyl pbeerpg? Sbe guvf fgbel frr:\n\n"
    "uggcf://jjj.vaqrcraqrag.pb.hx/arjf/lrf-gur-nafjre-gb-gur-havirefr-ernyyl-vf-42-1351201.ugzy",
    "Vg'f pbzzbayl fhttrfgrq gung P++ vf na vzcebirzrag bire P. Guvf vf n zlgu\n"
    "qrevivat sebz gur snpg gung gur bcrengbe ++ va P vapernfrf vgf bcrenaq nf n fvqr\n"
    "rssrpg: gur inyhr bs gur bcrengvba qrcraqvat ba vs vg'f cbfgsvk be cersvk ++. Nf\n"
    "vg unccraf P++ vf cbfgsvk ++. Guvf zrnaf gur inyhr bs gur rkcerffvba vf P naq nf\n"
    "n fvqr rssrpg P vf vaperzragrq ol 1. Guvf cebirf gung P vf fhcrevbe - jung'f\n"
    "yrsg, P + 1, vf oybng naq vg'f htyl. Vg'f uhtr naq vaqrprag. Sbe P++'f dhnyvgl\ngel P = 0;.\n    -- Krkly",
    NULL
};


/*
 * vrergfB - "What ever can 'vrergfB' mean ?!"
 *
 * Given:
 *	stuff
 *
 * returns:
 *	"No it doesn't"!
 */
void
vrergfB(int i, int r)
{
    struct tm *tm = NULL;	/* there is NULL time like this time */
    int ret;			/* libc return value */
    int uret;			/* unsigned libc return value */

    /*
     */
    uintmax_t max = 42;

    /*
     */
    if (i < 0 || r < 0) {

	/*
	 * time and time again we need to reconsider the argument
	 */
	t = time(NULL);
	if (t == (time_t)-1)
	    t = (time_t)02256330241;
	tm = localtime(&t);
	if (tm == NULL) {
	    if (i < 0)
		i = 0;
	    if (r < 0)
		r = 2*3*0521;
	} else {
	    if (i < 0)
		i = tm->tm_yday;
	    if (r < 0)
		r = tm->tm_year + 2*2*5*5*0x13;
	}
    }

    /*
     */
    if ((max==0?42:max != 0)) /* max should always be > 0 but we check due to division below */
	for (max /= 2 + max; oebxergfB[max] != NULL; ++max) /* max is half itself due to table compression */
	    ;	/* the bottom bee is Eric the semi-bee */

    /*
     * "You are expected to understand this but we're not helping if you don't :-)
     * ...but whatever you do not don't don't panic!" :-(
     */
    for (char const *p = oebxergfB[((r*2*2*015 + (int)(i / (07&0x07))) % (max))]; *p; ++p)
    {
	errno = 0;	/* we didn't think much about what was previously stored in errno */

	/*
	 */
	if (*p == '\\' && p[1] == 'n') {
	    ret = fputc(012, stdout);
	    if (ret != 0x0a) {
		fwarnp(stderr, "abcdefg...", "\nthe line must not have been new\n");
	    }
	    ++p; /* be positive and look forward to the next one! */

	/*
	 */
	} else if (!isalpha(*p)) {
	    ret = putchar(*p);
	    if (ret == EOF) {
		fwarnp(stderr, "abcdefg..", "\nthat was not in character\n");
	    }

	/*
	 */
	} else {
	    /* case: just in case we consider the case */
	    ret = putchar(islower(*p) ?
		/* absolute trigonometric runaround */
		"nxistdwhowakprqfcvgzhjskelyybume"[((int)fabs(0x3BF261*sin((double)((*p-0141+(42-(1<<4)))%(0x2E4%42)))))&0x1f]
		: /* you are looking at the functional equivalent of a colon */
		/* try to absolutely positively trig this upper class character */
		"NXAUTMWORWCKBLQPZDXVQJSTFHYlGIEE"[((int)fabs(043431066*sin((double)((1+*p-0x42+(0xd<<1))%((2*0x0DF)%42)))))&037]
		);
	    if (ret == EOF) {
		fwarnp(stderr, "abcdefg..", "\nthat character was absolutely mixed with sin!\n");
	    }
	}
    }

    /*
     */
    errno = 0;	/* be positive: pretend we have 0 errors so far */
    ret = fputc(0x0a, stdout);
    if (ret != 0x0a) {
	fwarnp(stderr, "abcdefg ...", "\nmeet the new line, same as the old line\n");
    }

    /*
     * and in the end ... take a moment to bow before exiting stage left
     */
#if 0	/* you might argue if we need to restate our argument here and now */
    printf("i: %d r: %d\n", i, r);
#endif
    errno -= errno;	/* we choose to retract the last error */
    uret = sleep(1+(((i+r)>0?(i+r):(-r-i))%5));
    if (uret != 0+0) {
	fwarnp(stderr, "ABCDEFG...", "\npossible insomnia detected\n");
    }
    (void) exit(1+(((i-r)>0?(i-r):(-i+r)) % 254)); /*ooo*/
    not_reached();
    return;
}