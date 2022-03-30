/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2021-2022 Julian Rüth
 *
 *  intervalxt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  intervalxt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with intervalxt. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#include <benchmark/benchmark.h>
#include <gmpxx.h>

#include "../src/impl/rational_affine_subspace.hpp"

using namespace intervalxt;

namespace intervalxt::test {

void HasNonZeroNonNegativeVector(benchmark::State& state) {
  // A difficult set of equations that came up when determining the orbit
  // closure of the (1, 3, 3, 3, 10) pentagon.
  const std::vector<std::vector<mpq_class>> equations = {
      {mpq_class(685610, 11), mpq_class(915726, 11), mpq_class(378678, 11), 31416, mpq_class(908492, 11), mpq_class(868822, 11), mpq_class(503596, 11), mpq_class(94920, 11), mpq_class(79848, 11), mpq_class(50590, 11), mpq_class(462252, 11), mpq_class(387364, 11), mpq_class(531818, 11), mpq_class(96928, 11), 8074, mpq_class(62730, 11), 2200, 13960, mpq_class(135252, 11), mpq_class(33834, 11), mpq_class(50212, 11), mpq_class(7918, 11), mpq_class(56368, 11), mpq_class(470164, 11), mpq_class(225020, 11), mpq_class(538696, 11), mpq_class(226318, 11), mpq_class(124650, 11), mpq_class(104954, 11), mpq_class(41210, 11), mpq_class(88776, 11), mpq_class(48948, 11), mpq_class(68706, 11), mpq_class(127982, 11), mpq_class(117464, 11), mpq_class(157134, 11), mpq_class(155544, 11), mpq_class(24174, 11), mpq_class(91684, 11), mpq_class(179364, 11), mpq_class(205802, 11), mpq_class(191616, 11), mpq_class(268318, 11), mpq_class(297486, 11), mpq_class(537906, 11), mpq_class(308986, 11), mpq_class(62648, 11), mpq_class(126392, 11), 22934, mpq_class(703244, 11), mpq_class(382058, 11), mpq_class(92666, 11), mpq_class(101444, 11), mpq_class(64516, 11), mpq_class(96928, 11), mpq_class(69566, 11), mpq_class(26780, 11), mpq_class(124556, 11), mpq_class(7708, 11)},
      {mpq_class(310408, 11), mpq_class(416000, 11), mpq_class(173414, 11), mpq_class(158436, 11), mpq_class(415278, 11), mpq_class(396190, 11), 20754, mpq_class(42960, 11), mpq_class(35466, 11), mpq_class(23990, 11), mpq_class(192754, 11), mpq_class(159090, 11), mpq_class(220872, 11), mpq_class(41966, 11), mpq_class(37324, 11), mpq_class(26680, 11), mpq_class(11666, 11), mpq_class(68116, 11), 5440, mpq_class(16196, 11), mpq_class(24138, 11), mpq_class(3592, 11), 2104, mpq_class(212416, 11), mpq_class(103846, 11), mpq_class(244626, 11), mpq_class(98998, 11), mpq_class(52912, 11), 4118, mpq_class(19220, 11), mpq_class(38972, 11), mpq_class(21908, 11), mpq_class(33562, 11), mpq_class(54790, 11), mpq_class(50742, 11), mpq_class(69830, 11), mpq_class(69288, 11), mpq_class(11472, 11), mpq_class(41202, 11), mpq_class(77882, 11), mpq_class(91994, 11), mpq_class(88222, 11), mpq_class(120090, 11), mpq_class(133196, 11), mpq_class(239254, 11), mpq_class(137494, 11), mpq_class(28170, 11), mpq_class(54248, 11), mpq_class(112968, 11), mpq_class(318848, 11), mpq_class(168272, 11), mpq_class(41452, 11), mpq_class(47060, 11), 2728, mpq_class(41966, 11), mpq_class(28118, 11), mpq_class(12422, 11), mpq_class(53924, 11), mpq_class(4138, 11)},
      {-mpq_class(495106, 11), -mpq_class(661182, 11), -mpq_class(273408, 11), -mpq_class(249488, 11), -mpq_class(655924, 11), -mpq_class(627320, 11), -mpq_class(363602, 11), -mpq_class(68532, 11), -mpq_class(57636, 11), -mpq_class(36494, 11), -mpq_class(333768, 11), -mpq_class(279732, 11), -mpq_class(384008, 11), -6362, -mpq_class(64154, 11), -mpq_class(45322, 11), -mpq_class(17500, 11), -mpq_class(110856, 11), -mpq_class(97622, 11), -2222, -3294, -mpq_class(5714, 11), -mpq_class(40718, 11), -mpq_class(339492, 11), -mpq_class(162472, 11), -mpq_class(388996, 11), -mpq_class(163402, 11), -mpq_class(90018, 11), -mpq_class(75802, 11), -mpq_class(29742, 11), -mpq_class(64118, 11), -mpq_class(35338, 11), -mpq_class(49594, 11), -mpq_class(92478, 11), -mpq_class(84844, 11), -mpq_class(113448, 11), -mpq_class(112296, 11), -mpq_class(17438, 11), -mpq_class(66230, 11), -mpq_class(129604, 11), -mpq_class(148636, 11), -mpq_class(138422, 11), -mpq_class(193818, 11), -mpq_class(214866, 11), -mpq_class(388490, 11), -mpq_class(223216, 11), -mpq_class(45266, 11), -mpq_class(91326, 11), -mpq_class(182196, 11), -mpq_class(507786, 11), -mpq_class(275906, 11), -6086, -mpq_class(73208, 11), -mpq_class(46590, 11), -6362, -mpq_class(50240, 11), -mpq_class(19348, 11), -mpq_class(89966, 11), -504},
      {-mpq_class(226392, 11), -mpq_class(303420, 11), -mpq_class(126476, 11), -mpq_class(115554, 11), -mpq_class(302864, 11), -mpq_class(288940, 11), -mpq_class(166514, 11), -mpq_class(31330, 11), -mpq_class(25886, 11), -mpq_class(17498, 11), -mpq_class(140740, 11), -mpq_class(116174, 11), -mpq_class(161276, 11), -mpq_class(30632, 11), -mpq_class(27250, 11), -mpq_class(19478, 11), -mpq_class(8506, 11), -mpq_class(49700, 11), -mpq_class(43660, 11), -mpq_class(11808, 11), -mpq_class(17596, 11), -238, -mpq_class(16900, 11), -14084, -mpq_class(75710, 11), -mpq_class(178404, 11), -mpq_class(72232, 11), -mpq_class(38612, 11), -mpq_class(33056, 11), -1274, -mpq_class(28442, 11), -mpq_class(15988, 11), -mpq_class(24460, 11), -3634, -mpq_class(37024, 11), -mpq_class(50948, 11), -mpq_class(50554, 11), -mpq_class(8366, 11), -mpq_class(30044, 11), -mpq_class(56818, 11), -mpq_class(67102, 11), -mpq_class(64326, 11), -mpq_class(87590, 11), -mpq_class(97146, 11), -mpq_class(174524, 11), -mpq_class(100278, 11), -mpq_class(20538, 11), -mpq_class(39580, 11), -mpq_class(82388, 11), -mpq_class(232550, 11), -mpq_class(122768, 11), -mpq_class(30226, 11), -mpq_class(34318, 11), -1990, -mpq_class(30632, 11), -mpq_class(20536, 11), -mpq_class(9054, 11), -mpq_class(39346, 11), -mpq_class(3016, 11)},
      {-mpq_class(4499152, 11), -mpq_class(6004208, 11), -mpq_class(2533440, 11), -mpq_class(2316976, 11), -mpq_class(6138720, 11), -mpq_class(5726656, 11), -mpq_class(3432400, 11), -57568, -mpq_class(513280, 11), -mpq_class(319888, 11), -mpq_class(2883504, 11), -mpq_class(2451472, 11), -mpq_class(3321760, 11), -mpq_class(643344, 11), -mpq_class(622384, 11), -mpq_class(445600, 11), -mpq_class(203168, 11), -mpq_class(1008672, 11), -mpq_class(843808, 11), -mpq_class(242096, 11), -mpq_class(316384, 11), -mpq_class(71920, 11), -mpq_class(360384, 11), -mpq_class(3014816, 11), -mpq_class(1531632, 11), -mpq_class(3506880, 11), -mpq_class(1454160, 11), -mpq_class(844080, 11), -mpq_class(682192, 11), -mpq_class(235008, 11), -mpq_class(593264, 11), -mpq_class(313952, 11), -mpq_class(420240, 11), -mpq_class(726544, 11), -56208, -mpq_class(1030352, 11), -mpq_class(1002080, 11), -mpq_class(136480, 11), -mpq_class(418656, 11), -mpq_class(1069264, 11), -mpq_class(1212832, 11), -mpq_class(1147056, 11), -mpq_class(1684864, 11), -mpq_class(1631488, 11), -mpq_class(3099232, 11), -mpq_class(1732240, 11), -mpq_class(251088, 11), -mpq_class(698272, 11), -mpq_class(1533584, 11), -mpq_class(4577200, 11), -mpq_class(2222512, 11), -mpq_class(411152, 11), -mpq_class(625936, 11), -mpq_class(435728, 11), -mpq_class(643344, 11), -mpq_class(438256, 11), -mpq_class(178048, 11), -mpq_class(630816, 11), -mpq_class(33584, 11)},
      {-mpq_class(1615536, 11), -mpq_class(2192016, 11), -mpq_class(892080, 11), -mpq_class(817552, 11), -mpq_class(2101696, 11), -mpq_class(2065264, 11), -mpq_class(1139632, 11), -mpq_class(217744, 11), -mpq_class(197264, 11), -mpq_class(136336, 11), -mpq_class(1099456, 11), -mpq_class(883984, 11), -mpq_class(1256960, 11), -19888, -mpq_class(170240, 11), -mpq_class(116960, 11), -mpq_class(34832, 11), -mpq_class(356768, 11), -mpq_class(342624, 11), -mpq_class(71664, 11), -mpq_class(135056, 11), -mpq_class(10768, 11), -mpq_class(123760, 11), -mpq_class(1142624, 11), -mpq_class(517184, 11), -mpq_class(1286272, 11), -mpq_class(538368, 11), -mpq_class(261680, 11), -21504, -mpq_class(120896, 11), -mpq_class(195968, 11), -10992, -mpq_class(189264, 11), -mpq_class(325712, 11), -mpq_class(331728, 11), -mpq_class(368160, 11), -mpq_class(372880, 11), -mpq_class(75824, 11), -mpq_class(291792, 11), -mpq_class(437152, 11), -mpq_class(539632, 11), -mpq_class(499920, 11), -58944, -75584, -mpq_class(1434720, 11), -mpq_class(831536, 11), -mpq_class(214784, 11), -mpq_class(330432, 11), -mpq_class(637328, 11), -mpq_class(1683504, 11), -mpq_class(1010848, 11), -mpq_class(297920, 11), -mpq_class(270992, 11), -mpq_class(151104, 11), -19888, -mpq_class(157504, 11), -mpq_class(61216, 11), -mpq_class(362368, 11), -3088},
      {mpq_class(3528848, 11), mpq_class(4713312, 11), mpq_class(1986848, 11), mpq_class(1816784, 11), mpq_class(4808944, 11), 408416, 244192, mpq_class(496816, 11), mpq_class(404320, 11), mpq_class(253424, 11), mpq_class(2272992, 11), mpq_class(1927952, 11), 237968, mpq_class(504768, 11), mpq_class(485120, 11), mpq_class(346992, 11), mpq_class(156224, 11), 72080, 60592, mpq_class(188864, 11), mpq_class(250352, 11), 4960, mpq_class(283072, 11), mpq_class(2369520, 11), mpq_class(1198480, 11), mpq_class(2752576, 11), mpq_class(1143776, 11), mpq_class(659104, 11), mpq_class(535760, 11), 17072, mpq_class(465056, 11), mpq_class(247392, 11), mpq_class(333792, 11), mpq_class(574608, 11), mpq_class(493344, 11), mpq_class(809712, 11), 71744, mpq_class(109328, 11), mpq_class(339792, 11), mpq_class(842992, 11), mpq_class(959520, 11), mpq_class(905936, 11), mpq_class(1324320, 11), mpq_class(1299312, 11), mpq_class(2456384, 11), mpq_class(1375248, 11), mpq_class(206112, 11), mpq_class(554080, 11), mpq_class(1209680, 11), mpq_class(3594768, 11), mpq_class(1759744, 11), mpq_class(334464, 11), mpq_class(495936, 11), mpq_class(341728, 11), 45888, mpq_class(344656, 11), mpq_class(139632, 11), mpq_class(506384, 11), mpq_class(28192, 11)},
      {mpq_class(842656, 11), mpq_class(1138048, 11), mpq_class(458336, 11), 38112, mpq_class(1073408, 11), mpq_class(1073008, 11), mpq_class(575632, 11), mpq_class(111072, 11), mpq_class(100976, 11), mpq_class(71536, 11), mpq_class(568704, 11), 41072, mpq_class(649168, 11), mpq_class(109504, 11), mpq_class(79312, 11), mpq_class(54368, 11), mpq_class(14944, 11), 16400, mpq_class(179792, 11), mpq_class(36320, 11), mpq_class(72784, 11), mpq_class(3312, 11), mpq_class(62896, 11), mpq_class(601472, 11), mpq_class(265984, 11), mpq_class(673760, 11), mpq_class(278656, 11), mpq_class(129088, 11), mpq_class(120112, 11), mpq_class(67824, 11), mpq_class(97872, 11), mpq_class(62272, 11), 9424, 16464, mpq_class(187728, 11), mpq_class(188128, 11), mpq_class(191648, 11), mpq_class(42208, 11), mpq_class(174928, 11), 21872, mpq_class(297920, 11), 25392, mpq_class(349456, 11), mpq_class(472848, 11), mpq_class(795600, 11), 42832, mpq_class(132336, 11), 16784, mpq_class(345200, 11), mpq_class(879232, 11), mpq_class(552688, 11), mpq_class(179856, 11), mpq_class(144784, 11), mpq_class(78048, 11), mpq_class(109504, 11), mpq_class(80464, 11), mpq_class(32656, 11), mpq_class(208528, 11), mpq_class(19472, 11)},
      {-mpq_class(1936824, 11), -mpq_class(2615776, 11), -99440, -90992, -mpq_class(2598720, 11), -mpq_class(2472392, 11), -mpq_class(1427960, 11), -mpq_class(263816, 11), -mpq_class(231520, 11), -mpq_class(154136, 11), -mpq_class(1248408, 11), -mpq_class(1024704, 11), -mpq_class(1430160, 11), -mpq_class(266328, 11), -21008, -mpq_class(164160, 11), -mpq_class(67576, 11), -mpq_class(432624, 11), -mpq_class(385816, 11), -mpq_class(96472, 11), -mpq_class(150688, 11), -mpq_class(20880, 11), -mpq_class(149528, 11), -mpq_class(1328152, 11), -mpq_class(643904, 11), -mpq_class(1522320, 11), -mpq_class(633040, 11), -mpq_class(332520, 11), -mpq_class(292704, 11), -mpq_class(125656, 11), -mpq_class(249848, 11), -mpq_class(145704, 11), -19480, -31128, -mpq_class(322296, 11), -40784, -mpq_class(446840, 11), -mpq_class(74600, 11), -mpq_class(255952, 11), -43608, -mpq_class(574744, 11), -mpq_class(542952, 11), -mpq_class(741648, 11), -mpq_class(830696, 11), -mpq_class(1497424, 11), -77400, -mpq_class(173576, 11), -mpq_class(340624, 11), -mpq_class(704928, 11), -mpq_class(1998880, 11), -mpq_class(1063624, 11), -mpq_class(256544, 11), -mpq_class(297272, 11), -mpq_class(187456, 11), -mpq_class(266328, 11), -mpq_class(181752, 11), -mpq_class(76816, 11), -mpq_class(339752, 11), -mpq_class(26968, 11)},
      {-mpq_class(241768, 11), -mpq_class(317912, 11), -mpq_class(130544, 11), -mpq_class(117648, 11), -mpq_class(325496, 11), -27728, -mpq_class(187592, 11), -mpq_class(36456, 11), -mpq_class(26680, 11), -mpq_class(14616, 11), -mpq_class(170888, 11), -mpq_class(151112, 11), -mpq_class(198384, 11), -mpq_class(37952, 11), -mpq_class(43976, 11), -mpq_class(30864, 11), -mpq_class(11920, 11), -mpq_class(57624, 11), -mpq_class(44032, 11), -mpq_class(13504, 11), -mpq_class(14208, 11), -mpq_class(4448, 11), -mpq_class(22784, 11), -mpq_class(161688, 11), -7056, -mpq_class(187536, 11), -mpq_class(79160, 11), -mpq_class(51488, 11), -mpq_class(38704, 11), -mpq_class(8968, 11), -mpq_class(34624, 11), -1392, -mpq_class(14776, 11), -mpq_class(42488, 11), -mpq_class(33688, 11), -mpq_class(54176, 11), -mpq_class(53320, 11), -mpq_class(4952, 11), -mpq_class(19720, 11), -mpq_class(62880, 11), -mpq_class(64320, 11), -mpq_class(56936, 11), -mpq_class(92688, 11), -7640, -mpq_class(169072, 11), -mpq_class(92936, 11), -mpq_class(11896, 11), -mpq_class(41632, 11), -mpq_class(82840, 11), -mpq_class(243736, 11), -mpq_class(123672, 11), -mpq_class(19816, 11), -mpq_class(29576, 11), -mpq_class(21744, 11), -mpq_class(37952, 11), -mpq_class(27496, 11), -mpq_class(7896, 11), -mpq_class(34536, 11), -mpq_class(24, 11)},
      {mpq_class(1388712, 11), mpq_class(1876552, 11), mpq_class(784816, 11), 65304, mpq_class(1862904, 11), mpq_class(1772776, 11), mpq_class(1023136, 11), mpq_class(188808, 11), mpq_class(166328, 11), mpq_class(110688, 11), mpq_class(895456, 11), mpq_class(734248, 11), mpq_class(1025696, 11), mpq_class(190776, 11), mpq_class(164656, 11), mpq_class(116920, 11), mpq_class(47896, 11), mpq_class(309888, 11), mpq_class(277128, 11), mpq_class(68864, 11), mpq_class(108232, 11), mpq_class(14912, 11), mpq_class(107136, 11), mpq_class(952640, 11), mpq_class(461704, 11), mpq_class(1091600, 11), mpq_class(454360, 11), mpq_class(237968, 11), mpq_class(209960, 11), mpq_class(90536, 11), mpq_class(178936, 11), mpq_class(104864, 11), mpq_class(154304, 11), mpq_class(245816, 11), mpq_class(231864, 11), 29272, mpq_class(320648, 11), mpq_class(53864, 11), mpq_class(184200, 11), mpq_class(343520, 11), mpq_class(412504, 11), mpq_class(389544, 11), mpq_class(531296, 11), mpq_class(596704, 11), mpq_class(1074960, 11), mpq_class(611088, 11), 11368, mpq_class(244472, 11), mpq_class(505904, 11), mpq_class(1433552, 11), mpq_class(763984, 11), mpq_class(184632, 11), mpq_class(213584, 11), mpq_class(134344, 11), mpq_class(190776, 11), 11840, mpq_class(55184, 11), mpq_class(244248, 11), mpq_class(19520, 11)},
      {mpq_class(207832, 11), mpq_class(274320, 11), mpq_class(112920, 11), 9272, mpq_class(281040, 11), 23920, mpq_class(161560, 11), mpq_class(31280, 11), mpq_class(23208, 11), mpq_class(13064, 11), mpq_class(145488, 11), mpq_class(128160, 11), mpq_class(168728, 11), mpq_class(32448, 11), mpq_class(37144, 11), mpq_class(26072, 11), mpq_class(10216, 11), mpq_class(49648, 11), mpq_class(38096, 11), mpq_class(11608, 11), mpq_class(12544, 11), mpq_class(3808, 11), mpq_class(19176, 11), mpq_class(139272, 11), mpq_class(67112, 11), mpq_class(161472, 11), mpq_class(67864, 11), mpq_class(43752, 11), mpq_class(33032, 11), mpq_class(7968, 11), mpq_class(29664, 11), 1208, mpq_class(13344, 11), mpq_class(35848, 11), mpq_class(28784, 11), mpq_class(46704, 11), mpq_class(46096, 11), mpq_class(4472, 11), mpq_class(16960, 11), mpq_class(53280, 11), mpq_class(55240, 11), mpq_class(49088, 11), mpq_class(79400, 11), mpq_class(72200, 11), mpq_class(144896, 11), mpq_class(79264, 11), mpq_class(10176, 11), mpq_class(35240, 11), mpq_class(71032, 11), mpq_class(210048, 11), 9632, mpq_class(16960, 11), mpq_class(25976, 11), mpq_class(18912, 11), mpq_class(32448, 11), mpq_class(23240, 11), mpq_class(6784, 11), mpq_class(29440, 11), mpq_class(320, 11)},
      {1432576, 1966592, 807424, 743936, 1906944, 1844992, 1041408, 196096, 183296, 129280, 1010176, 813056, 1155072, 206848, 157952, 108288, 32256, 331008, 312832, 65024, 121344, 11264, 111360, 1011968, 462080, 1139200, 486400, 238336, 212224, 105216, 177408, 110336, 166912, 271104, 278272, 340224, 341504, 69376, 232704, 364544, 461568, 422144, 557824, 694272, 1219840, 689408, 165376, 272384, 546560, 1501952, 873216, 235008, 246272, 141312, 206848, 144896, 50944, 301568, 33280},
      {-1687296, -2302976, -958720, -877312, -2275584, -2162432, -1260800, -239616, -211968, -137472, -1171456, -970240, -1345024, -248064, -221696, -154368, -50688, -397568, -348416, -81152, -130048, -18688, -139008, -1167360, -552192, -1330688, -569600, -304896, -267776, -103936, -227584, -130816, -181760, -304128, -290304, -403456, -405248, -65792, -215808, -414208, -495104, -445952, -629760, -710912, -1308672, -724736, -142080, -305920, -619520, -1752064, -953856, -215808, -267776, -161536, -248064, -173568, -63488, -303104, -23552},
      {-663040, -902144, -367872, -338176, -868352, -848128, -473600, -89344, -82432, -57600, -456448, -369152, -522240, -92928, -71680, -49664, -15360, -148992, -140288, -30208, -55040, -5120, -50944, -467968, -211712, -527616, -220672, -109056, -97024, -47616, -81152, -49920, -76544, -132096, -133888, -154112, -154624, -30976, -114176, -176128, -217600, -201216, -263168, -331776, -577792, -332032, -83200, -132608, -257792, -691968, -410880, -116224, -111104, -64512, -92928, -65792, -24320, -144384, -13824},
      {695552, 943360, 389632, 356096, 924928, 887808, 511232, 97536, 85504, 56064, 473088, 391680, 542976, 99840, 88832, 62208, 20736, 161024, 141312, 33536, 53504, 7168, 56320, 483072, 225280, 550144, 231168, 123392, 108544, 43008, 92160, 52736, 75520, 132096, 126464, 163584, 164608, 26880, 98304, 178432, 210688, 192512, 266240, 308992, 559360, 315648, 67584, 133120, 261376, 721408, 403712, 99328, 109568, 66304, 99840, 69888, 26368, 132608, 9728},
      {355264, 482688, 198656, 182464, 464256, 449088, 256512, 46144, 44864, 26240, 238464, 196864, 273728, 49152, 43968, 30912, 9024, 76992, 69568, 15616, 25856, 4416, 29440, 243712, 110400, 277440, 117120, 61824, 55616, 21504, 47552, 29184, 36608, 69376, 66688, 81856, 81856, 13504, 49856, 90496, 107072, 95616, 133056, 156928, 287680, 160512, 35264, 69376, 132800, 364608, 210944, 50752, 51328, 31808, 49152, 35264, 13696, 67712, 3200},
      {163264, 223040, 96192, 87360, 239552, 213632, 138112, 26496, 20224, 12480, 111744, 98048, 129216, 26944, 32000, 22464, 10624, 43328, 31104, 10432, 10752, 3392, 14208, 107648, 56896, 126464, 53696, 35008, 26816, 5760, 25728, 11328, 11264, 17984, 12928, 38848, 39296, 3328, 1920, 32192, 35072, 30080, 55808, 36992, 87808, 40704, -2624, 18432, 47936, 166912, 67072, 576, 21888, 16448, 26944, 17472, 4992, 11904, 448},
      {-338368, -455744, -188288, -171968, -449920, -429952, -250496, -46400, -40704, -25088, -225024, -188288, -258688, -48000, -45568, -32064, -11520, -76096, -65728, -16384, -24256, -4480, -27840, -231488, -108736, -264960, -110656, -61312, -52288, -19328, -45312, -25216, -32704, -62400, -57600, -77568, -77376, -11712, -42496, -85696, -99072, -90560, -128832, -141568, -262272, -146816, -29248, -62208, -123392, -347264, -190016, -42944, -48640, -31360, -48000, -33664, -12608, -59264, -3328},
      {-33728, -47872, -21824, -19968, -54144, -45376, -31360, -5696, -4672, -2944, -24064, -20864, -27776, -6080, -7232, -5056, -2752, -9792, -6912, -2432, -2368, -832, -3008, -21376, -12672, -25536, -11648, -7552, -5824, -1216, -5824, -2752, -2240, -1024, 64, -8704, -8768, -704, 2816, -4032, -5056, -3840, -9728, -2240, -11072, -2688, 3520, -1088, -7488, -34688, -9472, 3456, -4608, -3712, -6080, -3712, -896, 576, -64},
      {121344, 161728, 67456, 61184, 164896, 154688, 92832, 17504, 13824, 8640, 79104, 67584, 91168, 17632, 18368, 12992, 5504, 28192, 22848, 6496, 8256, 1856, 10080, 81952, 40320, 94720, 39200, 23264, 18816, 6144, 16672, 8224, 10528, 20416, 17408, 27616, 27456, 3424, 11936, 30016, 33312, 31136, 46336, 45248, 86560, 48192, 7584, 20256, 42240, 123968, 62144, 11872, 16608, 11488, 17632, 12064, 4416, 17952, 960},
      {24352, 33664, 14144, 13088, 32000, 30912, 17024, 2976, 3200, 2208, 16832, 13056, 19168, 3264, 1824, 1280, 320, 5248, 5472, 992, 2176, 96, 1888, 17056, 8000, 19200, 8512, 3680, 3776, 2144, 2912, 2240, 3488, 4800, 4992, 6080, 5984, 1344, 4352, 6016, 8000, 7488, 9216, 12352, 21024, 12192, 3072, 4704, 9472, 25600, 15008, 4384, 4320, 2400, 3264, 2336, 1056, 5344, 544},
      {-81952, -109728, -45760, -41600, -111104, -104480, -62368, -11680, -9536, -5920, -53856, -45696, -62016, -11872, -12064, -8512, -3456, -18944, -15648, -4256, -5664, -1216, -6816, -55488, -27072, -63968, -26688, -15520, -12768, -4320, -11232, -5760, -7360, -13952, -12160, -18784, -18688, -2464, -8416, -20160, -22720, -21056, -31072, -31136, -59264, -32896, -5408, -13856, -28736, -83872, -42752, -8384, -11392, -7712, -11872, -8160, -3008, -12512, -704},
      {-19424, -26592, -11168, -10272, -26016, -24768, -14176, -2560, -2464, -1664, -13152, -10592, -15040, -2720, -2080, -1472, -544, -4384, -4096, -928, -1600, -160, -1536, -13408, -6400, -15264, -6560, -3232, -3008, -1408, -2496, -1632, -2368, -3520, -3456, -4704, -4672, -864, -2784, -4736, -5920, -5504, -7328, -8704, -15488, -8768, -1888, -3488, -7168, -20224, -11104, -2784, -3168, -1920, -2720, -1888, -768, -3648, -320}};

  for (auto _ : state) {
    RationalAffineSubspace(equations, std::vector<mpq_class>(equations.size())).hasNonZeroNonNegativeVector();
  }
}
BENCHMARK(HasNonZeroNonNegativeVector);

}  // namespace intervalxt::test
