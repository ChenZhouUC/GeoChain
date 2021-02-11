import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd

sns.set(font='DejaVu Math TeX Gyre', style='darkgrid')


def parse_line(line_text):
    if '[$]' in line_text:
        return line_text.split(": ")[-1].split(" ")[0]
    elif '[%]' in line_text:
        return line_text.split(": ")[-1][:-2]
    # elif '[#]' in line_text:
    #     return line_text.split(": ")[-1][:-1]
    else:
        return False


if __name__ == "__main__":

    with open('../../logs/geochain.INFO', 'r') as f:
        lines = f.readlines()
        result_list = []
        counter = 0
        result_tmp = []
        for l in lines:
            this_line = parse_line(l)
            if this_line != False:
                counter += 1
                result_tmp.append(float(this_line))
                if counter == 4:
                    result_list.append(result_tmp)
                    counter = 0
                    result_tmp = []
    print(counter, result_list)
    data = pd.DataFrame(result_list, columns=["sweepline", "inters/segments(%)", "traversal", "inters/segments(%)"])
    df = data.iloc[:, [1, 0, 2]]
    print(df)

    with sns.color_palette('muted', 16):
        plot_1 = sns.regplot(x=df["inters/segments(%)"], y=df["sweepline"], ci=95, marker='*')
        plot_2 = sns.regplot(x=df["inters/segments(%)"], y=df["traversal"], ci=95, marker='o')
        plt.xlabel("Intersections / Segements (%)")
        plt.ylabel("Computation Time Cost")
        plt.legend(title='Algorithm', loc='upper right', labels=['PlaneSweeper', 'Traversal'])
        plt.show()
