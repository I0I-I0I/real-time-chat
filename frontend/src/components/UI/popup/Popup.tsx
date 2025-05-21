import cls from "@/utils/cls"
import styles from "./Popup.module.css"

interface PopupProps {
    children: React.ReactNode | string
    className?: string
    direction?: string
}

export const Popup = ({ children, className = "", direction = "down_top" }: PopupProps) => {
    return <div className={cls(styles.popup, className, styles[direction])}>{children}</div>
}
