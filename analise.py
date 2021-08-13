# Allan Cedric G. B. Alves da Silva - GRR20190351
# Gabriel N. Hishida do Nascimento - GRR20190361

import csv
import os

# LIKWID Markers
markers = {
    "gen-curve-matcoef": {},
    "LU-U-1": {},
    "LU-U-2": {},
    "LU-O-1": {},
    "LU-O-2": {}
}

# Arquivos das tabelas
tables_filename = [
    './Resultados/runtime-gen-curve-matcoef.tsv',
    './Resultados/l2missratio-gen-curve-matcoef.tsv',
    './Resultados/dpavx-gen-curve-matcoef.tsv',
    './Resultados/l3bandwith-gen-curve-matcoef.tsv',

    './Resultados/runtime-LU-U-1.tsv',
    './Resultados/l2missratio-LU-U-1.tsv',
    './Resultados/dpavx-LU-U-1.tsv',
    './Resultados/l3bandwith-LU-U-1.tsv',

    './Resultados/runtime-LU-U-2.tsv',
    './Resultados/l2missratio-LU-U-2.tsv',
    './Resultados/dpavx-LU-U-2.tsv',
    './Resultados/l3bandwith-LU-U-2.tsv',

    './Resultados/runtime-LU-O-1.tsv',
    './Resultados/l2missratio-LU-O-1.tsv',
    './Resultados/dpavx-LU-O-1.tsv',
    './Resultados/l3bandwith-LU-O-1.tsv',

    './Resultados/runtime-LU-O-2.tsv',
    './Resultados/l2missratio-LU-O-2.tsv',
    './Resultados/dpavx-LU-O-2.tsv',
    './Resultados/l3bandwith-LU-O-2.tsv'
]

# Agrupamento das tabelas
tables_per_func = [
    tables_filename[0:4],
    tables_filename[4:8],
    tables_filename[8:12],
    tables_filename[12:16],
    tables_filename[16:20]
]

# Cabeçalho das tabelas
for filename in tables_filename:
    if not os.path.isfile(filename):
        with open(filename, 'w') as table:
            tsv_writer = csv.writer(table, delimiter='\t')
            if "runtime" in filename:
                tsv_writer.writerow(['n', 'Runtime(ms)'])
            elif "l2missratio" in filename:
                tsv_writer.writerow(['n', 'L2 miss ratio'])
            elif "dpavx" in filename:
                tsv_writer.writerow(['n', 'DP (MFLOP/s)', 'AVX DP (MFLOP/s)'])
            else:
                tsv_writer.writerow(['n', 'L3 bandwidth (MBytes/s)'])

# Ordem da matriz
size = int(input())

line = input()
while line:

    # Decide o marker em questão
    if "TABLE,Region" in line:
        current = line.split()[1].split(",")[0]
    else:
        # Procura por informações relevantes
        for pattern in ["L2 miss ratio", "DP [MFLOP/s]", "AVX DP [MFLOP/s]", "RDTSC Runtime [s]",
                        "L3 bandwidth [MBytes/s]"]:
            if line.find(pattern) == 0:

                # Soma dos Runtime's
                if pattern == "RDTSC Runtime [s]":
                    markers[current][pattern] = markers[current].get(
                        pattern, 0) + eval(line.split(",")[1])
                else:
                    markers[current][pattern] = eval(line.split(",")[1])

    try:
        line = input()
    except EOFError:
        # Média dos Runtime's
        for key in markers.keys():
            markers[key]["RDTSC Runtime [s]"] /= 3
            markers[key]["RDTSC Runtime [s]"] *= 1000  # ms

        i = 0
        # Construindo as tabelas, para cada marker um conjunto de tabelas a serem preenchidas
        for key in markers.keys():
            for filename in tables_per_func[i]:
                with open(filename, 'a') as table:
                    tsv_writer = csv.writer(table, delimiter='\t')
                    if "runtime" in filename:
                        tsv_writer.writerow(
                            [str(size), markers[key]["RDTSC Runtime [s]"]])
                    elif "l2missratio" in filename:
                        tsv_writer.writerow(
                            [str(size), markers[key]["L2 miss ratio"]])
                    elif "dpavx" in filename:
                        tsv_writer.writerow([str(size),
                                             markers[key]["DP [MFLOP/s]"],
                                             markers[key]["AVX DP [MFLOP/s]"]])
                    else:
                        tsv_writer.writerow(
                            [str(size), markers[key]["L3 bandwidth [MBytes/s]"]])
            i += 1
        exit()
