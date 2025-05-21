import cls from "@/utils/cls"
import styles from "./Modal.module.css"

interface ModalProps {
    children: React.ReactNode
    className?: string
    exit?: boolean
    onExit?: () => void
}

export const Modal = ({ children, exit = false, onExit, className = "" }: ModalProps) => (
    <div className={cls(styles.modal, className, exit ? styles.exit : "")}>
        {exit && <button onClick={onExit} className={styles.exit_button} />}
        {children}
    </div>
)
