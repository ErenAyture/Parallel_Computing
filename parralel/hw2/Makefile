define my_important_task =
# script goes here
for number in 1 2 3 4 ; do \
    ./run.sh $$number ; \
done
#./run.sh
endef

my-important-task: ; $(value my_important_task)

.ONESHELL: