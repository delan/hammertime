SUBMISSION_CLEAN_FILES=log.full.*.txt *.o *.cnet *.log *.aux *.out *.toc

submission: report.pdf logs
	rm -f $(SUBMISSION_CLEAN_FILES)

report.pdf:
	pdflatex report.tex

logs:
	cnet -W TOPOLOGY -o log.node.%d.txt

cli:
	cnet -W TOPOLOGY

gui:
	cnet -g -O TOPOLOGY

clean:
	rm -f $(SUBMISSION_CLEAN_FILES) log.node.*.txt *.pdf
