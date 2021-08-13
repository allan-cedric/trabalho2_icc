#!/usr/bin/env python3

# Allan Cedric G. B. Alves da Silva - GRR20190351
# Gabriel N. H. do Nascimento - GRR20190361

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy.interpolate import make_interp_spline

# Relaciona os arquivos das tabelas com os nomes dos seus respectivos campos
filename_to_field = {
    "./Resultados/runtime-Aj-MatCoef.tsv": "Runtime(ms)",
    "./Resultados/l2missratio-Aj-MatCoef.tsv": "L2 miss ratio",
    "./Resultados/dpavx-Aj-MatCoef.tsv": "DP (MFLOP/s)",
    "./Resultados/l3bandwith-Aj-MatCoef.tsv": "L3 bandwidth (MBytes/s)",

    "./Resultados/runtime-It-LU-Unoptimized.tsv": "Runtime(ms)",
    "./Resultados/l2missratio-It-LU-Unoptimized.tsv": "L2 miss ratio",
    "./Resultados/dpavx-It-LU-Unoptimized.tsv": "DP (MFLOP/s)",
    "./Resultados/l3bandwith-It-LU-Unoptimized.tsv": "L3 bandwidth (MBytes/s)",

    "./Resultados/runtime-Aj-LU-Unoptimized.tsv": "Runtime(ms)",
    "./Resultados/l2missratio-Aj-LU-Unoptimized.tsv": "L2 miss ratio",
    "./Resultados/dpavx-Aj-LU-Unoptimized.tsv": "DP (MFLOP/s)",
    "./Resultados/l3bandwith-Aj-LU-Unoptimized.tsv": "L3 bandwidth (MBytes/s)",

    "./Resultados/runtime-It-LU-Optimized.tsv": "Runtime(ms)",
    "./Resultados/l2missratio-It-LU-Optimized.tsv": "L2 miss ratio",
    "./Resultados/dpavx-It-LU-Optimized.tsv": "DP (MFLOP/s)",
    "./Resultados/l3bandwith-It-LU-Optimized.tsv": "L3 bandwidth (MBytes/s)",

    "./Resultados/runtime-Aj-LU-Optimized.tsv": "Runtime(ms)",
    "./Resultados/l2missratio-Aj-LU-Optimized.tsv": "L2 miss ratio",
    "./Resultados/dpavx-Aj-LU-Optimized.tsv": "DP (MFLOP/s)",
    "./Resultados/l3bandwith-Aj-LU-Optimized.tsv": "L3 bandwidth (MBytes/s)",
}

# Declara par de tabelas que formam o mesmo gráfico
matching_tables = {
    "./Resultados/runtime-It-LU-Unoptimized.tsv": "./Resultados/runtime-It-LU-Optimized.tsv",
    "./Resultados/l2missratio-It-LU-Unoptimized.tsv": "./Resultados/l2missratio-It-LU-Optimized.tsv",
    "./Resultados/dpavx-It-LU-Unoptimized.tsv": "./Resultados/dpavx-It-LU-Optimized.tsv",
    "./Resultados/l3bandwith-It-LU-Unoptimized.tsv": "./Resultados/l3bandwith-It-LU-Optimized.tsv",

    "./Resultados/runtime-Aj-LU-Unoptimized.tsv": "./Resultados/runtime-Aj-LU-Optimized.tsv",
    "./Resultados/l2missratio-Aj-LU-Unoptimized.tsv": "./Resultados/l2missratio-Aj-LU-Optimized.tsv",
    "./Resultados/dpavx-Aj-LU-Unoptimized.tsv": "./Resultados/dpavx-Aj-LU-Optimized.tsv",
    "./Resultados/l3bandwith-Aj-LU-Unoptimized.tsv": "./Resultados/l3bandwith-Aj-LU-Optimized.tsv"
}

# Tabelas
tables = {}

# Le as tabelas
for filename in filename_to_field:
    tables[filename] = pd.read_csv(filename, delimiter='\t')

# --- Dados da ordem das matrizes (Eixo X) ---
# A coluna 'n' de todas as tabelas é igual
x_data = tables["./Resultados/runtime-Aj-MatCoef.tsv"]["n"] 
x_data = np.array(x_data)
x_data_smooth = np.linspace(x_data.min(), x_data.max(), x_data.max() - x_data.min() + 1)

# Gŕaficos das tabelas do marker Aj-MatCoef
for table in list(filename_to_field.keys())[:4]:

    # Tabela para DP e AVX
    if "dpavx" in table:
        for column in ("DP (MFLOP/s)", "AVX DP (MFLOP/s)"):
            # Valores do eixo Y
            y_data = tables[table][column].values
            y_data = np.array(y_data)

            # Spline para suavizar o desenho do gŕafico
            spl = make_interp_spline(x_data, y_data, k=2)
            y_data_smooth = spl(x_data_smooth)

            # Plota o desenho do gráfico
            label = "DP" if column == "DP (MFLOP/s)" else "AVX DP"
            plt.plot(x_data_smooth, y_data_smooth, label=label)

            # Plota os pontos críticos
            plt.scatter(x_data, y_data, s=10)

        plt.legend(fontsize='medium')
    else:
        # Valores do eixo Y
        y_data = tables[table][filename_to_field[table]].values
        y_data = np.array(y_data)

        # Spline para suavizar o desenho do gŕafico
        spl = make_interp_spline(x_data, y_data, k=2)
        y_data_smooth = spl(x_data_smooth)

        # Caso seja Runtime, usa escala logaritmica
        if "runtime" in table:
            plt.yscale("log")

        # Plota o desenho do gráfico
        plt.plot(x_data_smooth, y_data_smooth)

        # Plota os pontos críticos
        plt.scatter(x_data, y_data, s=10)

    # Rótulo do eixo X
    plt.xlabel("Tamanho (n)")

    title = "Ajuste de Curvas-MatCoef "
    if "dpavx" in table:
        ylabel = "MFLOP/s"
        title += "DP e AVX DP"
    else:
        ylabel = filename_to_field[table]
        title += filename_to_field[table].split()[0]

    # Rótulo do eixo Y
    plt.ylabel(ylabel)

    # Título do gŕafico
    plt.title(title)

    # Salva o gráfico no formato PNG
    img_file = "./Resultados/" + title.replace(" ","-").replace("/", "-") + ".png"
    plt.savefig(img_file, bbox_inches="tight")

    # Limpa o plot
    plt.clf()

# Gera gráfico para Fatoracao LU
for first, second in matching_tables.items():
    for table in (first, second):
        if "dpavx" in table:
            for column in ("DP (MFLOP/s)", "AVX DP (MFLOP/s)"):
                # Valores do eixo Y
                y_data = tables[table][column].values
                y_data = np.array(y_data)

                # Spline para suavizar o desenho do gŕafico
                spl = make_interp_spline(x_data, y_data, k=2)
                y_data_smooth = spl(x_data_smooth)

                # Plota o desenho do gráfico
                label = "Unoptimized" if table == first else "Optimized"
                label += " - DP" if column == "DP (MFLOP/s)" else " - AVX DP"
                plt.plot(x_data_smooth, y_data_smooth, label=label)

                # Plota os pontos críticos
                plt.scatter(x_data, y_data, s=10)
        else:
            # Valores do eixo Y
            y_data = tables[table][filename_to_field[table]].values
            y_data = np.array(y_data)

            # Spline para suavizar o desenho do gŕafico
            spl = make_interp_spline(x_data, y_data, k=2)
            y_data_smooth = spl(x_data_smooth)

            # Caso seja Runtime, usa escala logaritmica
            if "runtime" in table:
                plt.yscale("log")

            # Plota o desenho do gráfico
            label = "Unoptimized" if table == first else "Optimized"
            plt.plot(x_data_smooth, y_data_smooth, label=label)

            # Plota os pontos críticos
            plt.scatter(x_data, y_data, s=10)

    # Rótulo do eixo X
    plt.xlabel("Tamanho (n)")

    if "It-LU" in first:
        title = "Interpolação-LU "
    else:
        title = "Ajuste de Curvas-LU "

    if "dpavx" in first:
        ylabel = "MFLOP/s"
        title += "DP e AVX DP"
    else:
        ylabel = filename_to_field[first]
        title += filename_to_field[first].split()[0]

    # Rótulo do eixo Y
    plt.ylabel(ylabel)

    # Título do gŕafico
    plt.title(title)

    # Legenda
    plt.legend(fontsize='medium')

    # Salva a gráfico no formato PNG
    img_file = "./Resultados/" + title.replace(" ", "-").replace("/", "-") + ".png"
    plt.savefig(img_file, bbox_inches="tight")

    # Limpa o plot
    plt.clf()