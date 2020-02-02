#*****************************************************************
# File:   Makefile
# Author: Andrés Gavín Murillo, 716358
# Author: Abel Naya Forcano, 544125
# Date:   Enero 2020
# Coms:   Informática Gráfica - Trabajo 5: Photon mapping
#         Ejecutar "make" para compilar (o "make -f Makefile")
#         Ejecutar "make clean" para limpiar (borrar lo que genere)
#*****************************************************************

#---------------------------------------------------------
# macros
RM = /bin/rm -f  # para limpiar
CC = g++         # compilador
#---------------------------------------------------------
# compilación y enlazado ("linkado")
CPPFLAGS=-I. -march=native -O3 -std=c++14 -pthread -fmax-errors=1 # opciones compilación
#---------------------------------------------------------
# vars
EJEC = ray-trace
MAIN = main
BRDF = BRDF
COL = Color
GEO = Geometry
HC = HCoord
IM = Image
KD = KDTree
MA = Material
OB = Object
PM = PhotonMapping
PR = Progress
RA = Random
RE = Render
SC = Scene
TE = Texture
TR = Transform

all: ${EJEC}
#---------------------------------------------------------
# "linkar"
${EJEC}: ${MAIN}.o ${BRDF}.o ${COL}.o ${GEO}.o ${HC}.o ${IM}.o ${KD}.hpp ${MA}.o ${OB}.o ${PM}.o ${PR}.o ${RA}.o ${RE}.o ${SC}.o ${TE}.o ${TR}.o
	${CC} ${MAIN}.o ${BRDF}.o ${COL}.o ${GEO}.o ${HC}.o ${IM}.o ${KD}.hpp ${MA}.o ${OB}.o ${PM}.o ${PR}.o ${RA}.o ${RE}.o ${SC}.o ${TE}.o ${TR}.o -o ${EJEC} ${CPPFLAGS}

#---------------------------------------------------------
# compilar
${MAIN}.o: ${MAIN}.cpp ${BRDF}.hpp ${BRDF}.cpp ${COL}.hpp ${COL}.cpp ${GEO}.hpp ${GEO}.cpp ${HC}.hpp ${HC}.cpp ${IM}.hpp ${IM}.cpp ${KD}.hpp ${MA}.hpp ${MA}.cpp ${OB}.hpp ${OB}.cpp ${PM}.hpp ${PM}.cpp ${PR}.hpp ${PR}.cpp ${RA}.hpp ${RA}.cpp ${RE}.hpp ${RE}.cpp ${SC}.hpp ${SC}.cpp ${TE}.hpp ${TE}.cpp ${TR}.hpp ${TR}.cpp
	${CC} -c ${MAIN}.cpp ${CPPFLAGS}

${BRDF}.o: ${BRDF}.hpp ${BRDF}.cpp
	${CC} -c ${BRDF}.cpp ${CPPFLAGS}

${COL}.o: ${COL}.hpp ${COL}.cpp
	${CC} -c ${COL}.cpp ${CPPFLAGS}

${GEO}.o: ${GEO}.hpp ${GEO}.cpp
	${CC} -c ${GEO}.cpp ${CPPFLAGS}

${HC}.o: ${HC}.hpp ${HC}.cpp
	${CC} -c ${HC}.cpp ${CPPFLAGS}

${IM}.o: ${IM}.hpp ${IM}.cpp
	${CC} -c ${IM}.cpp ${CPPFLAGS}

${MA}.o: ${MA}.hpp ${MA}.cpp
	${CC} -c ${MA}.cpp ${CPPFLAGS}

${OB}.o: ${OB}.hpp ${OB}.cpp
	${CC} -c ${OB}.cpp ${CPPFLAGS}

${PM}.o: ${PM}.hpp ${PM}.cpp
	${CC} -c ${PM}.cpp ${CPPFLAGS}

${PR}.o: ${PR}.hpp ${PR}.cpp
	${CC} -c ${PR}.cpp ${CPPFLAGS}

${RA}.o: ${RA}.hpp ${RA}.cpp
	${CC} -c ${RA}.cpp ${CPPFLAGS}

${RE}.o: ${RE}.hpp ${RE}.cpp
	${CC} -c ${RE}.cpp ${CPPFLAGS}

${SC}.o: ${SC}.hpp ${SC}.cpp
	${CC} -c ${SC}.cpp ${CPPFLAGS}

${TE}.o: ${TE}.hpp ${TE}.cpp
	${CC} -c ${TE}.cpp ${CPPFLAGS}

${TR}.o: ${TR}.hpp ${TR}.cpp
	${CC} -c ${TR}.cpp ${CPPFLAGS}

#---------------------------------------------------------
# Cuidado con lo que se pone aquí, que se borra sin preguntar
clean:
	$(RM) ${MAIN}.o ${BRDF}.o ${COL}.o ${GEO}.o ${HC}.o ${IM}.o ${KD}.o ${MA}.o ${OB}.o ${PM}.o ${PR}.o ${RA}.o ${RE}.o ${SC}.o ${TE}.o ${TR}.o
