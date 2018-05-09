       START  0
FIRST  STL    RETADR
       LDB    #CLOOP
       BASE   CLOOP
CLOOP  +JSUB  ENDAAA
       LDA    CLOOP
       COMP   #0
       JEQ    ENDAAA
       +JSUB  FIRST
       J      CLOOP
	   SHIFTL X,3
ENDAAA LDA    CLOOP
       LDA    #3
	   END
