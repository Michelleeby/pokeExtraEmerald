EventScript_UseSurf:: @ 8271EA0
	goto_if_unset FLAG_BADGE05_GET, EventScript_EndUseSurf
	bufferpartymonnick 0, 0
	setfieldeffectargument 0, 0
	lockall
	msgbox gText_WantToUseSurf, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq EventScript_ReleaseUseSurf
	msgbox gText_PlayerUsedSurf, MSGBOX_DEFAULT
	dofieldeffect FLDEFF_USE_SURF
EventScript_ReleaseUseSurf:: @ 8271ED5
	releaseall
EventScript_EndUseSurf:: @ 8271ED6
	end
