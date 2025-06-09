# MPC-based-RTP-Protocol

This project presents a proof-of-concept implementation of Real-Time Pricing (RTP) in smart grids using Multi-Party Computation (MPC). The current version of implementation is an in-place simulation, focused primarily on evaluating the accuracy of the MPC-based RTP protocol. It does not yet incorporate the full communication pipeline among multiple parties. This allows us to isolate and evaluate the correctness and feasibility of secure computations in the context of real-time pricing. While the current estimates are based on simulated communication, they provide a meaningful baseline for evaluating accuracy in practical RTP deployments. The forthcoming implementation will include explicit synchronization and client-server communication, further improving the realistic conditions.

This implementation is generalized for smart grid pricing. However, it can also serve as a secure computation framework for Cyber-Physical Systems (CPS) where system dynamics, controller parameters, and signal bounds are well-defined. Just as in Kalman-filter-based control and state estimation in CPS, it is assumed that the system dynamics, consumer and supplier behaviors, and signal ranges are known a priori. Specifically, this includes: i) Known bounds on consumer pricing signals and demands, ii) Known bounds on supplier pricing and scheduled supply, iii) Predictable interaction models among system components. Given such assumptions, this MPC-based approach can be applied to any CPS scenario where preserving the privacy of individual users (e.g., consumers or suppliers or even control agents) is essential, while still enabling collaborative and highly accurate computation over private inputs. 

In case of any clarity or suggestions, please reach out to soumyadyuti.ghosh@gmail.com


This repository accompanies our paper accepted at **ASIACCS 2025**.
To cite our work, please use the following BibTeX entry:

```bibtex
@inproceedings{ghosh2025precision,
  title     = {Pay What You Spend! Privacy-Aware Real-Time Pricing with High Precision IEEE 754 Floating Point Division},
  author    = {Ghosh, Soumyadyuti and Harishma, Boyapally and Suresh, Ajith and Patra, Arpita and Mukhopadhyay, Debdeep and Dey, Soumyajit},
  booktitle = {Proceedings of the ACM Asia Conference on Computer and Communications Security (ASIACCS)},
  year      = {2025}
}
```
