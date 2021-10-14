/*
#############################################################################
# If you use BioFVM in your project, please cite BioFVM and the version     #
# number, such as below:                                                    #
#                                                                           #
# We solved the diffusion equations using BioFVM (Version 1.1.7) [1]        #
#                                                                           #
# [1] A. Ghaffarizadeh, S.H. Friedman, and P. Macklin, BioFVM: an efficient #
#    parallelized diffusive transport solver for 3-D biological simulations,#
#    Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730 #
#                                                                           #
#############################################################################
#                                                                           #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)   #
#                                                                           #
# Copyright (c) 2015-2017, Paul Macklin and the BioFVM Project              #
# All rights reserved.                                                      #
#                                                                           #
# Redistribution and use in source and binary forms, with or without        #
# modification, are permitted provided that the following conditions are    #
# met:                                                                      #
#                                                                           #
# 1. Redistributions of source code must retain the above copyright notice, #
# this list of conditions and the following disclaimer.                     #
#                                                                           #
# 2. Redistributions in binary form must reproduce the above copyright      #
# notice, this list of conditions and the following disclaimer in the       #
# documentation and/or other materials provided with the distribution.      #
#                                                                           #
# 3. Neither the name of the copyright holder nor the names of its          #
# contributors may be used to endorse or promote products derived from this #
# software without specific prior written permission.                       #
#                                                                           #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       #
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED #
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           #
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER #
# OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  #
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       #
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        #
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    #
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      #
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        #
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              #
#                                                                           #
#############################################################################
*/

#ifndef __BioFVM_microenvironment_h__
#define __BioFVM_microenvironment_h__

#include "BioFVM_mesh.h"
#include "BioFVM_agent_container.h"
#include "BioFVM_MultiCellDS.h"

namespace BioFVM{

/* and now some gradients */ 
typedef std::vector<double> gradient; 

/*! /brief   */

class Basic_Agent; 

class Microenvironment
{
 private:
	friend std::ostream& operator<<(std::ostream& os, const Microenvironment& S);  

	/*! For internal use and accelerations in solvers */ 
	std::vector< std::vector<double> > temporary_density_vectors1; 
	/*! For internal use and accelerations in solvers */ 
	std::vector< std::vector<double> > temporary_density_vectors2; 
	
	/*! for internal use in bulk source/sink solvers */
	std::vector< std::vector<double> > bulk_source_sink_solver_temp1; 
	std::vector< std::vector<double> > bulk_source_sink_solver_temp2; 
	std::vector< std::vector<double> > bulk_source_sink_solver_temp3; 
	bool bulk_source_sink_solver_setup_done; 

	
	/*! stores pointer to current density solutions. Access via operator() functions. */ 
	//std::vector< std::vector<double> >* p_density_vectors;
	// commenting out because PGI complains when its private
	
	std::vector< std::vector<gradient> > gradient_vectors; 
	std::vector<bool> gradient_vector_computed; 

	
	/*! helpful for solvers -- resize these whenever adding/removing substrates */ 
	std::vector<double> one; 
	std::vector<double> zero; 
	std::vector<double> one_half; 
	std::vector<double> one_third; 
	
	/*! for internal use in diffusion solvers : these make the solvers safe across microenvironments ""*/ 
	std::vector< std::vector<double> > thomas_temp1; 
	std::vector< std::vector<double> > thomas_temp2; 
	std::vector<double> thomas_constant1x; 
	std::vector<double> thomas_constant1y; 
	std::vector<double> thomas_constant1z; 
	std::vector<double> thomas_neg_constant1x; 
	std::vector<double> thomas_neg_constant1y; 
	std::vector<double> thomas_neg_constant1z; 
	bool thomas_setup_done; 
	int thomas_i_jump; 
	int thomas_j_jump; 
	int thomas_k_jump; 
	std::vector<double> thomas_constant1; 
	std::vector<double> thomas_constant1a; 
	std::vector<double> thomas_constant2;
	std::vector<double> thomas_constant3;
	std::vector<double> thomas_constant3a;
	std::vector< std::vector<double> > thomas_denomx;
	std::vector< std::vector<double> > thomas_cx;
	std::vector< std::vector<double> > thomas_denomy;
	std::vector< std::vector<double> > thomas_cy;
	std::vector< std::vector<double> > thomas_denomz;
	std::vector< std::vector<double> > thomas_cz;
	bool diffusion_solver_setup_done; 
	
	// on "resize density" type operations, need to extend all of these 
	
	/*
	std::vector<int> dirichlet_indices; 
	std::vector< std::vector<double> > dirichlet_value_vectors; 
	std::vector<bool> dirichlet_node_map; 
	*/
	std::vector< std::vector<double> > dirichlet_value_vectors; 
	std::vector<bool> dirichlet_activation_vector; 	
 public:

	std::vector< std::vector<double> >* p_density_vectors; // must be public for PGI Compiler

	/* variables for GPU  */
	bool openacc_data_bool; // bool for openacc data transfer
	double ** gpu_p_density_vectors; // gpu copy of p_density_vectors
	double ** gpu_thomas_denomx; // gpu copy of thomas_denomx
	double ** gpu_thomas_denomy; // gpu copy of thomas_denomy
	double ** gpu_thomas_denomz; // gpu copy of thomas_denomz
	double * gpu_thomas_constant1; // gpu copy of thomas_constant1
	int * gpu_thomas_i_jump; // gpu copy of thomas_i_jump
	int * gpu_thomas_j_jump; // gpu copy of thomas_j_jump
	int * gpu_thomas_k_jump; // gpu copy of thomas_k_jump
	double ** gpu_thomas_cx; // gpu copy of thomas_cx
	double ** gpu_thomas_cy; // gpu copy of thomas_cy
	double ** gpu_thomas_cz; // gpu copy of thomas_cz
	double ** gpu_dirichlet_value_vectors; // gpu copy of dirichlet_value_vectors
	bool * gpu_dirichlet_activation_vector; //gpu copy of dirichlet_activation_vector
	bool * gpu_voxels_is_dirichlet; //mesh.voxels[iteration].is_Dirichlet value copy, mesh size
	int * sizes_p_density_vectors;
	int * sizes_thomas_denomx;
	int * sizes_thomas_denomy;
	int * sizes_thomas_denomz;
	int * sizes_thomas_cx;
	int * sizes_thomas_cy;
	int * sizes_thomas_cz;
	int * sizes_dirichlet_value_vectors;
	int sizes_thomas_constant1;

	int num_dirichlet;

	/* functions for GPU  */
	void axpy_acc( double* y, double* a, double* x, int size ); //added size to axpy, passes in size of the first array for ease
	void naxpy_acc( double* y, double* a, double* x, int size ); //added size to naxpy, passes in size of the first array for ease

	void transfer_2D();
	void transfer_3D();

	void translate_vector_to_array();
	void translate_array_to_vector();

	void apply_dirichlet_conditions_GPU(void);
	void x_diffusion_GPU_2D();
	void y_diffusion_GPU_2D();

	void x_diffusion_GPU_3D();
	void y_diffusion_GPU_3D();
	void z_diffusion_GPU_3D();

	int get_size_p1(); // gets the size of p_density_vectors
	int get_size_p2(); // gets the size of p_density_vectors[0] = 3


	/*! The mesh for the diffusing quantities */ 
	Cartesian_Mesh mesh;
	Agent_Container * agent_container;	
	std::string spatial_units; 
	std::string time_units; 
	std::string name; 

	// diffusing entities 
	std::vector< std::string > density_names; 
	std::vector< std::string > density_units; 
 
	// coefficients 
	std::vector< double > diffusion_coefficients; 
	std::vector< double > decay_rates; 
	
	std::vector< std::vector<double> > supply_target_densities_times_supply_rates; 
	std::vector< std::vector<double> > supply_rates; 
	std::vector< std::vector<double> > uptake_rates; 
	void update_rates( void ); 
	
	Microenvironment(); 
	Microenvironment(std::string name);
	
	void (*diffusion_decay_solver)( Microenvironment&, double); 
	void (*bulk_supply_rate_function)( Microenvironment* pMicroenvironment, int voxel_index, std::vector<double>* write_destination );
	void (*bulk_supply_target_densities_function)( Microenvironment* pMicroenvironment, int voxel_index, std::vector<double>* write_destination );
	void (*bulk_uptake_rate_function)( Microenvironment* pMicroenvironment, int voxel_index, std::vector<double>* write_destination );
		
	/*! functions to simplify size queries */ 
	
	unsigned int number_of_densities( void ); 
	unsigned int number_of_voxels( void ); 
	unsigned int number_of_voxel_faces( void ); 

 	
	void auto_choose_diffusion_decay_solver( void ); 
	
	// Only use this on non-Cartesian meshes. It's a fail-safe. 
	void resize_voxels( int new_number_of_voxes ); 
	
	void resize_space( int x_nodes, int y_nodes, int z_nodes ); 
	void resize_space( double x_start, double x_end, double y_start, double y_end, double z_start, double z_end , int x_nodes, int y_nodes, int z_nodes );  
	void resize_space( double x_start, double x_end, double y_start, double y_end, double z_start, double z_end , double dx_new , double dy_new , double dz_new ); 
	void resize_space_uniform( double x_start, double x_end, double y_start, double y_end, double z_start, double z_end , double dx_new ); 

	void resize_densities( int new_size );  
	void add_density( void ); 
	void add_density( std::string name , std::string units );
	void add_density( std::string name , std::string units, double diffusion_constant, double decay_rate ); 

	void set_density( int index , std::string name , std::string units ); 
	void set_density( int index , std::string name , std::string units , double diffusion_constant , double decay_rate ); 

	int find_density_index( std::string name ); 

	#pragma acc routine
	int voxel_index( int i, int j, int k ); 
	std::vector<unsigned int> cartesian_indices( int n ); 
	
	int nearest_voxel_index( std::vector<double>& position ); 
	std::vector<unsigned int> nearest_cartesian_indices( std::vector<double>& position ); 
	Voxel& nearest_voxel( std::vector<double>& position ); 
	Voxel& voxels( int voxel_index );
	std::vector<double>& nearest_density_vector( std::vector<double>& position );  
	std::vector<double>& nearest_density_vector( int voxel_index );  

	/*! access the density vector at  [ X(i),Y(j),Z(k) ] */
	std::vector<double>& operator()( int i, int j, int k ); 
	/*! access the density vector at  [ X(i),Y(j),0 ]  -- helpful for 2-D problems */
	std::vector<double>& operator()( int i, int j );  
	/*! access the density vector at [x,y,z](n) */
	std::vector<double>& operator()( int n );  
	
	std::vector<gradient>& gradient_vector(int i, int j, int k); 
	std::vector<gradient>& gradient_vector(int i, int j ); 
	std::vector<gradient>& gradient_vector(int n );  
	
	std::vector<gradient>& nearest_gradient_vector( std::vector<double>& position ); 

	void compute_all_gradient_vectors( void ); 
	void compute_gradient_vector( int n );  
	void reset_all_gradient_vectors( void ); 
	
	/*! access the density vector at  [ X(i),Y(j),Z(k) ] */
	std::vector<double>& density_vector( int i, int j, int k ); 
	/*! access the density vector at  [ X(i),Y(j),0 ]  -- helpful for 2-D problems */
	std::vector<double>& density_vector( int i, int j ); 

	/*! access the density vector at [x,y,z](n) */
	std::vector<double>& density_vector( int n ); 

	#pragma acc routine
	double& density_vector_GPU( int n );
       	
	/*! advance the diffusion-decay solver by dt time */
	void simulate_diffusion_decay( double dt ); 
	
	/*! advance the source/sink solver by dt time */
	void simulate_bulk_sources_and_sinks( double dt ); 
	
	// use the supplied list of cells
	void simulate_cell_sources_and_sinks( std::vector<Basic_Agent*>& basic_agent_list , double dt ); 
	// use the global list of cells 
	void simulate_cell_sources_and_sinks( double dt ); 
	
	void display_information( std::ostream& os ); 
	
	void add_dirichlet_node( int voxel_index, std::vector<double>& value ); 
	void update_dirichlet_node( int voxel_index , std::vector<double>& new_value ); 
	void update_dirichlet_node( int voxel_index , int substrate_index , double new_value );
	void remove_dirichlet_node( int voxel_index ); 
	void apply_dirichlet_conditions( void ); 

	void set_substrate_dirichlet_activation( int substrate_index , bool new_value ); 
	double get_substrate_dirichlet_activation( int substrate_index ); 
	
	bool& is_dirichlet_node( int voxel_index ); 

	friend void diffusion_decay_solver__constant_coefficients_explicit( Microenvironment& S, double dt ); 
	friend void diffusion_decay_solver__constant_coefficients_explicit_uniform_mesh( Microenvironment& S, double dt ); 

	friend void diffusion_decay_solver__constant_coefficients_LOD_3D( Microenvironment& S, double dt ); 
	friend void diffusion_decay_solver__constant_coefficients_LOD_2D( Microenvironment& S, double dt ); 
	
	// GPU functions
	friend void diffusion_decay_solver__constant_coefficients_LOD_3D_GPU( Microenvironment& S, double dt ); 
	friend void diffusion_decay_solver__constant_coefficients_LOD_2D_GPU( Microenvironment& S, double dt ); 
	
	friend void diffusion_decay_explicit_uniform_rates( Microenvironment& M, double dt );
	
	void write_to_matlab( std::string filename );
	void write_mesh_to_matlab( std::string filename ); // not yet written 
	void write_densities_to_matlab( std::string filename ); // not yet written 
	
	void write_to_xml( std::string xml_filename , std::string data_filename ); // not yet written
	void read_from_matlab( std::string filename ); // not yet written 
	void read_from_xml( std::string filename ); // not yet written 
};

extern void diffusion_decay_solver__constant_coefficients_explicit( Microenvironment& S, double dt ); 
extern void diffusion_decay_solver__constant_coefficients_explicit_uniform_mesh( Microenvironment& S, double dt ); 

extern void diffusion_decay_solver__variable_coefficients_explicit( Microenvironment& S, double dt ); 
extern void diffusion_decay_solver__variable_coefficients_explicit_uniform_mesh( Microenvironment& S, double dt ); 


extern void diffusion_decay_solver__constant_coefficients_LOD_3D( Microenvironment& S, double dt ); 
extern void diffusion_decay_solver__constant_coefficients_LOD_2D( Microenvironment& S, double dt ); 

extern void diffusion_decay_solver__variable_coefficients_LOD_3D( Microenvironment& S, double dt ); 
extern void diffusion_decay_solver__variable_coefficients_LOD_2D( Microenvironment& S, double dt ); 

extern void diffusion_decay_source_sink_solver__constant_coefficients_LOD_3D( Microenvironment& S, double dt );

void zero_function( std::vector<double>& position, std::vector<double>& input , std::vector<double>* destination );
void one_function( std::vector<double>& position, std::vector<double>& input , std::vector<double>* destination );

void zero_function( Microenvironment* pMicroenvironment, int voxel_index, std::vector<double>* write_destination );
void one_function( Microenvironment* pMicroenvironment, int voxel_index, std::vector<double>* write_destination );

void set_default_microenvironment( Microenvironment* M );
Microenvironment* get_default_microenvironment( void ); 

class Microenvironment_Options
{
 private:
 
 public: 
	Microenvironment* pMicroenvironment;
	std::string name; 
 
	std::string time_units; 
	std::string spatial_units; 
	double dx;
	double dy; 
	double dz; 
	
	bool outer_Dirichlet_conditions; 
	std::vector<double> Dirichlet_condition_vector; 
	std::vector<bool> Dirichlet_activation_vector; 
	
	std::vector<double> initial_condition_vector; 
	
	bool simulate_2D; 
	std::vector<double> X_range; 
	std::vector<double> Y_range; 
	std::vector<double> Z_range; 
	
	Microenvironment_Options(); 
	
	bool calculate_gradients; 
	
	bool use_oxygen_as_first_field;
	
	bool track_internalized_substrates_in_each_agent; 	
};

extern Microenvironment_Options default_microenvironment_options; 
extern Microenvironment microenvironment;

void initialize_microenvironment( void ); 

};

#endif
