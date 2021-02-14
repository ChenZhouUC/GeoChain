import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
from copy import deepcopy

sns.set(font='DejaVu Math TeX Gyre', style='darkgrid')


def parse_line(line_text):
    if '[$]' in line_text:
        return line_text.split(": ")[-1].split(" ")[0]
    elif '[%]' in line_text:
        return line_text.split(": ")[-1][:-2]
    elif '[#]' in line_text:
        return line_text.split(": ")[-1][:-1]
    else:
        return False


if __name__ == "__main__":

    hue_dict = {
        0: "PlaneSweep",
        1: "TrivialTraversal",
        2: "OptimizedTraversal",
    }
    with open('../../logs/geochain.INFO', 'r') as f:
        lines = f.readlines()
        result_list = []
        counter = 0
        hue_counter = 0
        result_tmp = []
        for l in lines:
            this_line = parse_line(l)
            if this_line != False:
                counter += 1
                result_tmp.append(float(this_line))
                if counter % 3 == 0:
                    result_tmp.append(hue_dict[hue_counter % len(hue_dict)])
                    result_list.append(result_tmp)
                    hue_counter += 1
                    result_tmp = []
    print(counter, result_list)
    data = pd.DataFrame(result_list, columns=["timecost", "intersection", "intersections/segments(%)", "algorithm"])
    df = data.iloc[:, [3, 2, 0, 1]]
    print(df)
    diff_df = deepcopy(df[df['algorithm'] != hue_dict[1]])
    diff_df.reset_index(inplace=True, drop=True)
    diff_df.loc[diff_df['algorithm'] == hue_dict[0], 'intersection'] = np.array(diff_df.loc[diff_df['algorithm'] == hue_dict[0], 'intersection']) - np.array(df[df['algorithm'] == hue_dict[1]]['intersection'])
    diff_df.loc[diff_df['algorithm'] == hue_dict[2], 'intersection'] = np.array(diff_df.loc[diff_df['algorithm'] == hue_dict[2], 'intersection']) - np.array(df[df['algorithm'] == hue_dict[1]]['intersection'])
    print(diff_df)

    with sns.color_palette('muted', 16):
        plot_1 = sns.lmplot(x="intersections/segments(%)", y="timecost", hue="algorithm", data=df, ci=95, markers=['*', 'o', '+'], legend=True, legend_out=False, order=2)
        plt.xlabel("Intersections / Segements (%)")
        plt.ylabel("Computation Time Cost")
        plt.show()

        plot_2 = sns.lmplot(x="intersections/segments(%)", y="intersection", hue="algorithm", data=diff_df, ci=95, markers=['*', '+'], legend=True, legend_out=False, order=1)
        plt.xlabel("Intersections / Segements (%)")
        plt.ylabel("Intersections Number Diff")
        plt.show()
