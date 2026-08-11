/* ==========================================================================
   Timer420 — GitHub Pages Interactive App & Doc Renderer
   ========================================================================== */

document.addEventListener('DOMContentLoaded', () => {
  initSimulator();
  initDocBrowser();
  initHotkeys();
});

/* --------------------------------------------------------------------------
   1. Timer Overlay Simulator
   -------------------------------------------------------------------------- */
let timerState = {
  totalSeconds: 25 * 60,
  remainingSeconds: 25 * 60,
  isRunning: false,
  intervalId: null,
  mode: 'minimal' // 'minimal', 'progress', 'edge'
};

function initSimulator() {
  const displayEl = document.getElementById('sim-display-time');
  const targetEl = document.getElementById('sim-target-time');
  const progressFillEl = document.getElementById('sim-progress-fill');
  const percentTextEl = document.getElementById('sim-percent-text');
  const toggleBtn = document.getElementById('sim-toggle-btn');
  const resetBtn = document.getElementById('sim-reset-btn');
  const modeBtns = document.querySelectorAll('.sim-mode-btn');
  const overlayBox = document.getElementById('sim-overlay-box');
  const edgeBorderBox = document.getElementById('sim-edge-border');

  function formatTime(seconds) {
    const hrs = Math.floor(seconds / 3600);
    const mins = Math.floor((seconds % 3600) / 60);
    const secs = seconds % 60;
    
    if (hrs > 0) {
      return `${String(hrs).padStart(2, '0')}:${String(mins).padStart(2, '0')}:${String(secs).padStart(2, '0')}`;
    }
    return `${String(mins).padStart(2, '0')}:${String(secs).padStart(2, '0')}`;
  }

  function updateUI() {
    if (displayEl) displayEl.textContent = formatTime(timerState.remainingSeconds);
    
    const elapsed = timerState.totalSeconds - timerState.remainingSeconds;
    const pct = Math.min(100, Math.max(0, (elapsed / timerState.totalSeconds) * 100)).toFixed(1);
    
    if (progressFillEl) progressFillEl.style.width = `${pct}%`;
    if (percentTextEl) percentTextEl.textContent = `${pct}% Elapsed`;

    // Target deadline calculation
    const now = new Date();
    const targetTime = new Date(now.getTime() + timerState.remainingSeconds * 1000);
    const timeStr = targetTime.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
    if (targetEl) targetEl.textContent = `Target Finish: ${timeStr}`;

    if (toggleBtn) {
      toggleBtn.textContent = timerState.isRunning ? 'Pause' : 'Start';
      toggleBtn.classList.toggle('sim-btn-primary', !timerState.isRunning);
    }
  }

  function startTimer() {
    if (timerState.isRunning) return;
    timerState.isRunning = true;
    timerState.intervalId = setInterval(() => {
      if (timerState.remainingSeconds > 0) {
        timerState.remainingSeconds--;
        updateUI();
      } else {
        pauseTimer();
        alert('🔔 Timer Completed! (Audible Chime + Toast Simulated)');
      }
    }, 1000);
    updateUI();
  }

  function pauseTimer() {
    timerState.isRunning = false;
    if (timerState.intervalId) {
      clearInterval(timerState.intervalId);
      timerState.intervalId = null;
    }
    updateUI();
  }

  function resetTimer(seconds = 25 * 60) {
    pauseTimer();
    timerState.totalSeconds = seconds;
    timerState.remainingSeconds = seconds;
    updateUI();
  }

  // Toggle Play/Pause
  if (toggleBtn) {
    toggleBtn.addEventListener('click', () => {
      if (timerState.isRunning) pauseTimer();
      else startTimer();
    });
  }

  // Reset
  if (resetBtn) {
    resetBtn.addEventListener('click', () => resetTimer(timerState.totalSeconds));
  }

  // Mode switching
  modeBtns.forEach(btn => {
    btn.addEventListener('click', () => {
      modeBtns.forEach(b => b.classList.remove('active'));
      btn.classList.add('active');
      const mode = btn.dataset.mode;
      timerState.mode = mode;

      if (overlayBox && edgeBorderBox) {
        overlayBox.style.display = (mode === 'edge') ? 'none' : 'block';
        edgeBorderBox.style.display = (mode === 'edge') ? 'block' : 'none';

        const progressSection = document.getElementById('sim-progress-section');
        if (progressSection) {
          progressSection.style.display = (mode === 'progress') ? 'block' : 'none';
        }
      }
    });
  });

  // Duration preset buttons
  document.querySelectorAll('.sim-preset-btn').forEach(btn => {
    btn.addEventListener('click', () => {
      const mins = parseInt(btn.dataset.mins, 10);
      resetTimer(mins * 60);
    });
  });

  updateUI();
}

/* --------------------------------------------------------------------------
   2. Dynamic Documentation Browser & Search
   -------------------------------------------------------------------------- */
const DOCS_LIST = [
  { id: 'USER_GUIDE.md', title: '📖 User Guide', category: 'Getting Started' },
  { id: 'BUILDING.md', title: '🛠️ Building & Toolchains', category: 'Getting Started' },
  { id: 'ARCHITECTURE.md', title: '📐 Architecture Overview', category: 'Technical Specs' },
  { id: 'TIMER_BEHAVIOR.md', title: '⏱️ Timer Behavior Spec', category: 'Technical Specs' },
  { id: 'CONFIGURATION.md', title: '⚙️ Configuration Reference', category: 'Technical Specs' },
  { id: 'TESTING.md', title: '🧪 Testing Strategy', category: 'Quality Assurance' },
  { id: 'RELEASE_PROCESS.md', title: '🚀 Release Process', category: 'Deployment' },
  { id: 'ARCHITECTURE_ANALYSIS.md', title: '🔬 Architecture Analysis', category: 'Deep Dives' },
  { id: 'MIGRATION_PLAN.md', title: '🗺️ Migration Plan', category: 'Deep Dives' },
  { id: 'DEVELOPMENT.md', title: '💻 Development Guidelines', category: 'Deep Dives' }
];

function initDocBrowser() {
  const menuContainer = document.getElementById('docs-menu');
  const contentArea = document.getElementById('docs-content');
  const searchInput = document.getElementById('docs-search-input');

  if (!menuContainer || !contentArea) return;

  function renderMenu(filter = '') {
    menuContainer.innerHTML = '';
    const categories = {};

    DOCS_LIST.forEach(doc => {
      if (filter && !doc.title.toLowerCase().includes(filter.toLowerCase()) && !doc.id.toLowerCase().includes(filter.toLowerCase())) {
        return;
      }
      if (!categories[doc.category]) categories[doc.category] = [];
      categories[doc.category].push(doc);
    });

    Object.keys(categories).forEach(cat => {
      const catHeader = document.createElement('li');
      catHeader.className = 'docs-nav-title';
      catHeader.textContent = cat;
      menuContainer.appendChild(catHeader);

      categories[cat].forEach(doc => {
        const li = document.createElement('li');
        li.className = 'docs-menu-item';
        
        const btn = document.createElement('button');
        btn.className = 'docs-menu-btn';
        btn.dataset.doc = doc.id;
        btn.innerHTML = `<span>${doc.title}</span>`;

        btn.addEventListener('click', () => {
          document.querySelectorAll('.docs-menu-btn').forEach(b => b.classList.remove('active'));
          btn.classList.add('active');
          loadDoc(doc.id);
        });

        li.appendChild(btn);
        menuContainer.appendChild(li);
      });
    });

    // Default select first item
    const firstBtn = menuContainer.querySelector('.docs-menu-btn');
    if (firstBtn && !menuContainer.querySelector('.docs-menu-btn.active')) {
      firstBtn.classList.add('active');
      loadDoc(firstBtn.dataset.doc);
    }
  }

  async function loadDoc(docId) {
    contentArea.innerHTML = '<div style="color:var(--text-muted); font-style:italic;">Loading documentation...</div>';
    
    try {
      const response = await fetch(`./${docId}`);
      if (!response.ok) throw new Error(`HTTP error ${response.status}`);
      const markdown = await response.text();

      if (window.marked) {
        contentArea.innerHTML = window.marked.parse(markdown);
      } else {
        contentArea.innerText = markdown;
      }
    } catch (err) {
      contentArea.innerHTML = `
        <div style="padding:1rem; border:1px solid var(--rose); background:rgba(244,63,94,0.1); border-radius:8px;">
          <h3 style="color:var(--rose); margin-bottom:0.5rem;">Documentation File: ${docId}</h3>
          <p style="color:var(--text-muted);">View technical documentation file directly in repository at <code>docs/${docId}</code>.</p>
        </div>
      `;
    }
  }

  if (searchInput) {
    searchInput.addEventListener('input', (e) => {
      renderMenu(e.target.value);
    });
  }

  renderMenu();
}

/* --------------------------------------------------------------------------
   3. Hotkeys Interaction
   -------------------------------------------------------------------------- */
function initHotkeys() {
  document.querySelectorAll('.hotkey-card').forEach(card => {
    card.addEventListener('click', () => {
      card.style.transform = 'scale(0.97)';
      card.style.borderColor = 'var(--cyan)';
      setTimeout(() => {
        card.style.transform = 'none';
        card.style.borderColor = 'var(--border-subtle)';
      }, 200);
    });
  });
}
