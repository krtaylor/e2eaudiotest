# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

from lava_test.core.installers import TestInstaller
from lava_test.core.parsers import TestParser
from lava_test.core.runners import TestRunner
from lava_test.core.tests import Test

INSTALLSTEPS = ['git clone git://git.linaro.org/people/kurt-r-taylor/e2eaudiotest.git']
DEPS = ['git-core', 'libasound2-dev', 'libfftw3-dev', 'gcc']
DEFAULT_OPTIONS = ""
RUNSTEPS = ['cd e2eaudiotest; gcc testfreq.c utils_alsa.c -lasound -lfftw3 -o testfreq ; ./e2eaudiotest.sh']
PATTERN = "^(?P<test_case_id>\w+):\W+(?P<result>\w+)\W+(?P<measurement>\d+)\W+sinewave"

e2einst = TestInstaller(INSTALLSTEPS, deps=DEPS)
e2erun = TestRunner(RUNSTEPS,default_options=DEFAULT_OPTIONS)
e2eparser = TestParser(PATTERN,
               appendall={'units':'Hz'})
testobj = Test(test_id="e2eaudiotest", installer=e2einst,
                                  runner=e2erun, parser=e2eparser)
