# Allan Cedric G. B. Alves da Silva - GRR20190351
# Gabriel N. Hishida do Nascimento - GRR20190361

import csv
import os

# LIKWID Markers
markers = {
    "Aj-MatCoef": {},
    "It-LU-Unoptimized": {},
    "Aj-LU-Unoptimized": {},
    "It-LU-Optimized": {},
    "Aj-LU-Optimized": {}
}

# Arquivos das tabelas
tables_filename = [
    './Resultados/runtime-Aj-MatCoef.tsv',
    './Resultados/l2missratio-Aj-MatCoef.tsv',
    './Resultados/dpavx-Aj-MatCoef.tsv',
    './Resultados/l3bandwith-Aj-MatCoef.tsv',

    './Resultados/runtime-It-LU-Unoptimized.tsv',
    './Resultados/l2missratio-It-LU-Unoptimized.tsv',
    './Resultados/dpavx-It-LU-Unoptimized.tsv',
    './Resultados/l3bandwith-It-LU-Unoptimized.tsv',

    './Resultados/runtime-Aj-LU-Unoptimized.tsv',
    './Resultados/l2missratio-Aj-LU-Unoptimized.tsv',
    './Resultados/dpavx-Aj-LU-Unoptimized.tsv',
    './Resultados/l3bandwith-Aj-LU-Unoptimized.tsv',

    './Resultados/runtime-It-LU-Optimized.tsv',
    './Resultados/l2missratio-It-LU-Optimized.tsv',
    './Resultados/dpavx-It-LU-Optimized.tsv',
    './Resultados/l3bandwith-It-LU-Optimized.tsv',

    './Resultados/runtime-Aj-LU-Optimized.tsv',
    './Resultados/l2missratio-Aj-LU-Optimized.tsv',
    './Resultados/dpavx-Aj-LU-Optimized.tsv',
    './Resultados/l3bandwith-Aj-LU-Optimized.tsv'
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
                markers[current][pattern] = (markers[current].get(pattern, 0) +
                                            eval(line.split(",")[1]))
    try:
        line = input()
    except EOFError:

        # Média aritmética simples de valores acumulados
        for key in markers.keys():

            if "Aj-MatCoef" in key: # Caso seja métricas da função gen_curve_matcoef()
                
                # Todas as métricas menos 'RDTSC Runtime [s]' aparecem 2 vezes.
                for k in markers[key].keys():
                    if k != "RDTSC Runtime [s]":
                        markers[key][k] /= 2
                markers[key]["RDTSC Runtime [s]"] /= 6 # Aparece 6 vezes   

            else:
                markers[key]["RDTSC Runtime [s]"] /= 3 # Aparece 3 vezes
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
