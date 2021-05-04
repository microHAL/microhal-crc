import json
import subprocess
import matplotlib.pyplot as plt
import csv


def runLatexToPDF(filename):
    subprocess.run(["pdflatex", "-synctex=1", "-interaction=nonstopmode ", filename])


def generateBarGraphPlotData(result):
    filedata = {}
    for crc in result:
        for impl in crc['implementation']:
            if impl not in filedata:
                filedata[impl] = {}
            filedata[impl][crc["CRC"]['name']] = crc['implementation'][impl] / 1000000
    print(filedata)
    print(filedata['BitShift'].keys())
    with open('data.csv', 'w') as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=['implementation'] + list(filedata['BitShift'].keys()))
        writer.writeheader()
        for implementation in filedata:
            tmp = filedata[implementation]
            tmp.update({'implementation': implementation})
            writer.writerow(tmp)


def main():
    proc = subprocess.Popen('../build/tests/multiplatformPerformance/multiplatformTest', stdout=subprocess.PIPE)
    stdout, stderr = proc.communicate()
    j = json.loads(stdout)
    implementations = ["BitShift", "BitShiftLsb", "Table256", "Table256Lsb"]
    result = {}
    for crc in j:
        for implementation in implementations:
            if implementation not in result:
                result[implementation] = {'x': [], 'y': []}
            result[implementation]['x'].append(crc["CRC"]["polyLength"])
            result[implementation]['y'].append(crc["implementation"][implementation])

    print(j)
    generateBarGraphPlotData(j)
    runLatexToPDF('report.tex')


if __name__ == '__main__':
    main()
