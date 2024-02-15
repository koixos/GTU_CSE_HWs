transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/and_4bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/alu.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/mux81_32bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/and41_32bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/or81_32bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/and_32bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/or_32bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/and_8bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/and_16bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/or_4bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/or_8bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/or_16bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/xor_4bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/xor_8bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/xor_16bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/xor_32bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/nor_4bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/nor_8bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/nor_16bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/nor_32bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/comparator_32bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/not_4bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/not_8bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/not_16bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/not_32bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/comparator_4bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/comparator_2bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/comparator_8bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/comparator_16bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/less_than_32bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/cla_4bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/cla_2bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/prop_gen_generator.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/cla_8bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/cla_16bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/cla_32bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/subtractor_32bit.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/mod.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/mod_cu.v}
vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/mod_dp.v}

vlog -vlog01compat -work work +incdir+C:/Project1 {C:/Project1/my_test_bench.v}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cyclonev_ver -L cyclonev_hssi_ver -L cyclonev_pcie_hip_ver -L rtl_work -L work -voptargs="+acc"  my_test_bench

add wave *
view structure
view signals
run -all
